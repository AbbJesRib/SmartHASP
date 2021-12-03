#!/usr/bin/python3
'Main file for getting and relaying mqtt messages'
import json
from typing import List
from fastapi import FastAPI
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware
from starlette.websockets import WebSocket, WebSocketDisconnect
import uvicorn
from pysondb import db
from time import time
from fastapi_mqtt import FastMQTT, MQQTConfig


database = db.getDb("db.json")
# run with > uvicorn main:app --reload

app = FastAPI(docs_url="/SMARTHASP/docs", redoc_url=None, openapi_url="/SMARTHASP/openapi.json")


mqtt_config = MQQTConfig(host = "api.easyprint.abbgymnasiet.se", port=6969)

fast_mqtt = FastMQTT(
    config=mqtt_config
)
fast_mqtt.init_app(app)

origins = [
    "*"
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


# start with: uvicorn main:app --reload


class ConnectionManager:
    def __init__(self):
        self.active_connections: List[WebSocket] = []

    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections.append(websocket)

    def disconnect(self, websocket: WebSocket):
        self.active_connections.remove(websocket)

    async def send_personal_message(self, message: str, websocket: WebSocket):
        await websocket.send_text(message)

    async def broadcast(self, message: dict):
        for connection in self.active_connections:
            await connection.send_json(message)


manager = ConnectionManager()


@app.get("/SMARTHASP/logs")
def get_logs(n: int = 10):
    return database.getAll()[-n:]


@app.post("/SMARTHASP/set_lock")
def set_lock(unlock: bool):
    fast_mqtt.publish(("un" if unlock else "") + "lock", "")
    return "Sending " + ("un" if unlock else "") + " to lock"


@app.websocket("/SMARTHASP/ws")
async def websocket_endpoint(websocket: WebSocket):
    await manager.connect(websocket)
    try:
        await websocket.send_json({
            "type": "info",
            "message": "welcome!"
        })
        while True:
            data = await websocket.receive_text()
            # await manager.send_personal_message(f"You wrote: {data}", websocket)
            # await manager.broadcast(f"Client says: {data}")

    except WebSocketDisconnect:
        manager.disconnect(websocket)
        # await manager.broadcast(f"Client left the chat")




webroot = "../Frontend/smart-hasp/dist/"
@app.get("/SMARTHASP", response_class=HTMLResponse)
def read_root():
    return open(f"{webroot}index.html").read()

app.mount("/SMARTHASP/", StaticFiles(directory=webroot), name="static")


@fast_mqtt.on_connect()
def on_connect(client, flags, rc, properties):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("le_lock")
    client.subscribe("le_grind")
    client.subscribe("state")

@fast_mqtt.on_message()
async def on_message(client, topic, payload, qos, properties):
    print(topic+" "+str(payload))
    res = json.loads(payload.decode())

    if topic == "le_lock":
        database.add({
            "ts": round(time()),
            "event": {
                "thing": "lock",
                "state": True if res["state"] else False
            }
        })
    elif topic == "le_grind":
        database.add({
            "ts": round(time()),
            "event": {
                "thing": "grind",
                "state": True if res["state"] else False
            }
        })
    elif topic == "state":
        mess = {
          "grind": True if res["grind"] else False,
          "lock": True if res["lock"] else False
        }
        print(mess)
        await manager.broadcast(mess)
    

    # unlocked
    # locked
    # opend
    # closed


if __name__ == "__main__":

    uvicorn.run("main:app", host='0.0.0.0', port=6970, reload=True, debug=False)

'Main file for getting and relaying mqtt messages'
import asyncio
import json
from threading import Thread
import threading
from pydantic import BaseModel
from typing import List, Optional
from fastapi import FastAPI
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware
from starlette.websockets import WebSocket, WebSocketDisconnect
import uvicorn
from threading import Thread
import paho.mqtt.client as mqtt
from pysondb import db
from time import process_time, time

database = db.getDb("db.json")
# run with > uvicorn main:app --reload

app = FastAPI()

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

    async def broadcast(self, message: str):
        for connection in self.active_connections:
            await connection.send_json(message)


manager = ConnectionManager()


@app.get("/logs")
def get_logs(n: int = 10):


    return [{"msg": "hej", "ts": 16258552254}]



@app.websocket("/ws")
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




# @app.get("/", response_class=HTMLResponse)
# def read_root():
#     return open(f"{webroot}index.html").read()

# webroot = "webview/dist/"
# app.mount("/", StaticFiles(directory=webroot), name="static")



def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("le_lock")
    client.subscribe("le_grind")
    client.subscribe("state")

def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    res = json.loads(msg.payload.decode())

    if msg.topic == "le_lock":
        res["state"]
        database.add({
            "ts": round(time()),
            "event": {
                "thing": "lock",
                "state": True if res["state"] else False
            }
        })
    elif msg.topic == "le_grind":
        database.add({
            "ts": round(time()),
            "event": {
                "thing": "grind",
                "state": True if res["state"] else False
            }
        })
    elif msg.topic == "state":
        mess = {
          "grind": True if res["grind"] else False,
          "lock": True if res["lock"] else False
        }
        print(mess)
        # await manager.broadcast(json.dumps(mess))
    

    # unlocked
    # locked
    # opend
    # closed


if __name__ == "__main__":
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect("localhost", 6969, 60)
    client.loop_start()
    uvicorn.run("main:app", host='0.0.0.0', port=6970, reload=True, debug=False)

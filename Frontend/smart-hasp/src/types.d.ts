export interface lockEvent {
    thing: "lock" | "grind",
    state: Boolean
}

export interface fetchMessage {
    ts: number,
    event: lockEvent,
    id: Number
}

export interface Message {
    message: "Gate opened" | "Gate closed" | "Gate unlocked" | "Gate locked",
    time: String,
    date: String
}
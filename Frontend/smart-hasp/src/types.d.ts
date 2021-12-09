export interface lockEvent {
    thing: 'lock' | 'grind';
    state: boolean;
}

export interface fetchMessage {
    ts: number;
    event: lockEvent;
    id: Number;
}

export interface Message {
    id: Number;
    message: String;
    time: String;
    date: String;
}

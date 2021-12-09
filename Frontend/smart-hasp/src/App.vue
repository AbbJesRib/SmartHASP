<template>
  <div id="app">
    <close-button :websocketdata="websocketdata"></close-button>
    <log-component></log-component>
  </div>
</template>

<script lang="ts">
import Vue from "vue";
import closeButton from "./components/closeButton.vue";
import logComponent from "./components/logComponent.vue";
import * as types from './types';


export default Vue.extend({
  name: "App",
  components: {
    closeButton,
    logComponent,
  },
  data: () => ({
    websocket: (undefined as unknown) as WebSocket,
    websocketdata: (undefined as unknown) as types.lockEvent

  }),
  created() {
    this.websocket = new WebSocket("wss://api.easyprint.abbgymnasiet.se/SMARTHASP/ws")

    this.websocket.onmessage = this.onMsg

    this.websocket.onopen = function(event) {
      console.log(event)
      console.log("Connected to websocket")
    }
  },
  methods: {
    onMsg(event: {data: string}) {
      this.websocketdata = JSON.parse(event.data)
      console.log(this.websocketdata)
    }
  },
});
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>

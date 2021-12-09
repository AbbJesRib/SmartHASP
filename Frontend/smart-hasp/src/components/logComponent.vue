<template>
  <div>
    <virtual-list
      style="height: 360px; overflow-y: auto"
      :data-key="'id'"
      :data-sources="messageLog"
      :data-component="messageComponent"
    />
  </div>
</template>

<script lang='ts'>
import message from "./messageComponent.vue";
import Vue, { PropType } from "vue";
import VirtualList from "vue-virtual-scroll-list";
import * as types from "../types";

const axios = require("axios");

export default Vue.extend({
  name: "logComponent",
  data() {
    return {
      messageComponent: message,
      messageLog: Array<types.Message>()
      };
  },
  components: { "virtual-list": VirtualList },
  async mounted() {
    const res = await axios.get("https://api.easyprint.abbgymnasiet.se/SMARTHASP/logs");
    const items: types.fetchMessage[] = res.data;
    console.log("items", items);

    const dates: Date[] = [];
    items.forEach((item) => {
      const timestamp: number = item.ts;
      const date = new Date(timestamp * 1000);
      dates.push(date);
    });

    // const messageLog: types.Message[] = []

    for (let i = 0; i < items.length; i++) {
      let currentDate = dates[i];
      let event = items[i].event;
      let gateMessage: String = "";
      switch (event.thing) {
        case "lock": 
          if (event.state) {
            gateMessage = "Gate unlocked";
          } else {
            gateMessage = "Gate locked";
          }
          break;
        case "grind":
          if (event.state) {
            gateMessage = "Gate opened";
          } else {
            gateMessage = "Gate closed";
          }
          break;
        default:
          break;
      }
      const time = currentDate.getHours()+":"+((currentDate.getMinutes() < 10) === true ? "0" : "")+currentDate.getMinutes();
      const date = currentDate.getDate()+"/"+(currentDate.getMonth()+1);
      const logMessage: types.Message = {
        id: items[i].id,
        message: gateMessage,
        time: time,
        date: date
      }
      this.messageLog.push(logMessage);
    }
    this.messageLog.reverse();
    console.log(this.messageLog);

  },
});
</script>
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
//import * as axios from 'axios';
import VirtualList from "vue-virtual-scroll-list";
import * as types from "../types";

const axios = require("axios");

export default Vue.extend({
  name: "logComponent",
  data() {
    return {
      messageComponent: message,
      messageLog: Array,
    };
  },
  components: { "virtual-list": VirtualList },
  async mounted() {
    const res = await axios.get("https://api.easyprint.abbgymnasiet.se/SMARTHASP/logs");
    const items: types.fetchMessage[] = res.data;
    console.log("items", items);

    const dates: Date[] = [];
    var coolBool = false;
    items.forEach((item) => {
      coolBool = true;
      const timestamp: number = item.ts;
      const date = new Date(timestamp * 1000);
      dates.push(date);
    });

    console.log(dates);

    const messageLog: types.Message[] = []

    for (let i = 0; i < items.length; i++) {
      let currentDate = dates[i];
      let event = items[i].event;
      switch (event.thing) {
        case "lock": 
          if (event.state) {
            const message = "Gate unlocked";
          } else {
            const message = "Gate locked";
          }
          break;
        case "grind":
          if (event.state) {
            const message = "Gate opened";
          } else {
            const message = "Gate closed";
          }
          break;
        default:
          break;
      }
      const time = currentDate.getHours()+":"+currentDate.getMinutes();
      const date = currentDate.getDate();
    }
  },
});
</script>
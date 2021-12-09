<template>
  <div>
    <virtual-list
      class="list"
      style="height: 360px; overflow-y: auto"
      :data-key="'id'"
      :data-sources="messageLog"
      :data-component="messageComponent"
      :estimate-size="50"
    />
  </div>
</template>

<script lang='ts'>
import message from './messageComponent.vue';
import Vue from 'vue';
import VirtualList from 'vue-virtual-scroll-list';
import * as types from '../types';
import axios from 'axios';

export default Vue.extend({
  name: 'logComponent',
  data() {
    return {
      messageComponent: message,
      messageLog: Array<types.Message>(),
    };
  },
  components: { 'virtual-list': VirtualList },
  async mounted() {
    const res = await axios.get('https://api.easyprint.abbgymnasiet.se/SMARTHASP/logs');
    const items: types.fetchMessage[] = res.data;

    const dates: Date[] = [];
    items.forEach((item) => {
      const timestamp: number = item.ts;
      const date = new Date(timestamp * 1000);
      dates.push(date);
    });

    const months = [
      'Jan',
      'Feb',
      'Mar',
      'Apr',
      'May',
      'Jun',
      'Jul',
      'Aug',
      'Sep',
      'Oct',
      'Nov',
      'Dec',
    ];

    for (let i = 0; i < items.length; i++) {
      const currentDate = dates[i];
      const event = items[i].event;
      let gateMessage: string = '';
      switch (event.thing) {
        case 'lock':
          if (event.state) {
            gateMessage = 'Gate unlocked';
          } else {
            gateMessage = 'Gate locked';
          }
          break;
        case 'grind':
          if (event.state) {
            gateMessage = 'Gate opened';
          } else {
            gateMessage = 'Gate closed';
          }
          break;
        default:
          break;
      }
      const time =
        currentDate.getHours() +
        ':' +
        (currentDate.getMinutes() < 10 === true ? '0' : '') +
        currentDate.getMinutes();
      const date = currentDate.getDate() + ' ' + months[currentDate.getMonth()];

      const logMessage: types.Message = {
        id: items[i].id,
        message: gateMessage,
        time,
        date,
      };
      this.messageLog.push(logMessage);
    }
    this.messageLog.reverse();
  },
});
</script>

<style>
#app {
  font-family: "Avenir", Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 1em;
  padding: 1em;
}
.list {
  border: 2px solid steelblue;
  border-radius: 3px;
}
</style>
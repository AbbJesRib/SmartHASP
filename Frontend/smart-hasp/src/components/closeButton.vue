<template>
  <div>
    <img
      v-if="gateUnlocked"
      class="selector"
      src="../assets/closeButton.png"
      @click="switchButton()"
      width="360px"
      :style="{opacity: buttonOpacity}"
    />
    <img
      v-else
      class="selector"
      src="../assets/openButton.png"
      @click="switchButton()"
      width="360px"
      :style="{opacity: buttonOpacity}"
    />
  </div>
</template>

<script lang='ts'>
import Vue from 'vue';
import * as types from '../types';

import axios from 'axios';

export default Vue.extend({
  name: 'closeButton',
  props: {
    websocketdata: Object,
  },
  data: () => ({
    // gateOpen: true,
    localGateUnlocked: true,
  }),
  methods: {
    sayHello() {
      console.log('Hello world!');
    },
    switchButton() {
        if (this.buttonOpacity == 0.5) return
        setTimeout(() => {
            this.localGateUnlocked = this.gateUnlocked
        }, 3000);
      this.localGateUnlocked = !this.localGateUnlocked
      axios.post(
        'https://api.easyprint.abbgymnasiet.se/SMARTHASP/set_lock',
        {},
        { params: { unlock: !this.gateUnlocked } },
      );
    },
  },
  computed: {
    buttonOpacity() {
        if (this.localGateUnlocked == this.gateUnlocked) {
            return 1
        }
        return 0.5
    },
    gateUnlocked() {
        const data: types.lockEvent = this.websocketdata
      if (data) {
          if (data.thing === "lock") {
            this.localGateUnlocked = data.state;
            return data.state;
          }
      }
          return false;
    },
  },
  // watch: {
  //     websocketdata:
  // },
});
</script>

<style scoped>
.selector {
    -webkit-user-drag: none;
    user-select: none;
    -moz-user-select: none;
    -webkit-user-select: none;
    -ms-user-select: none;
}
</style>
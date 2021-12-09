<template>
  <div>
    <img
      v-if="gateUnlocked"
      src="../assets/closeButton.png"
      @click="switchButton()"
      style="width: 360px"
    />
    <img
      v-else
      src="../assets/openButton.png"
      @click="switchButton()"
      style="width: 360px"
    />
  </div>
</template>

<script lang="ts">
import Vue from "vue";
import * as types from "../types";

import axios from "axios";

export default Vue.extend({
  name: "closeButton",
  props: {
    websocketdata: Object,
  },
  data: () => ({
    // gateOpen: true,
  }),
  methods: {
    sayHello() {
      console.log("Hello world!");
    },
    switchButton() {
      // this.gateOpen = !this.gateOpen
      axios.post(
        "https://api.easyprint.abbgymnasiet.se/SMARTHASP/set_lock",
        {},
        { params: { unlock: !this.gateUnlocked } }
      );
    },
  },
  computed: {
    gateUnlocked() {
        const data: types.lockEvent = this.websocketdata
      if (data) {
          if (data.thing == "lock") {
            return data.state;
          }
          return false;
      }
    },
  },
  // watch: {
  //     websocketdata:
  // },
  mounted() {},
});
</script>

<style scoped>
</style>
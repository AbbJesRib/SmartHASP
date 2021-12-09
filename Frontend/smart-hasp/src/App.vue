<template>
  <div id="app">
    <close-button></close-button>
    <div style="height: 60px"></div>
    <log-component></log-component>
  </div>
</template>

<script lang="ts">
import Vue from 'vue';
import closeButton from './components/closeButton.vue';
import logComponent from './components/logComponent.vue';

export default Vue.extend({
  name: 'App',
  components: {
    closeButton,
    logComponent,
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

<template>
  <div class='item'>
    <div class='id'>
      {{ source.message }} - {{ source.time }} {{ source.date }}
    </div>
  </div>
</template>

<script lang='ts'>
import Vue, { PropType } from 'vue';
import * as types from '../types';

export default Vue.extend({
  name: 'message',
  props: {
    index: {
      type: Number,
    },
    source: {
      type: Object as PropType<types.Message>,
      default() {
        return {};
      },
    },
  },
});
</script>

<style scoped>
.item {
  display: flex;
  flex-direction: column;
  border-bottom: 1px solid lightgrey;
  padding: 1em;
}
</style>

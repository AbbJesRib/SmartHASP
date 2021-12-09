<template>
  <div>{{ index }} - {{ source.message }} - {{ source.time }} {{ source.date }}</div>
</template>

<script lang="ts">

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
        return {

        };
      },
    },
  },
});
</script>
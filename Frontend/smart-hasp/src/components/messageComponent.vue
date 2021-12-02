<template>
  <div>{{message.uid}} - {{ source.text }}</div>
</template>

<script>

export default {
  name: "message-component",
  props: {
    message: Object,
  },
};
</script>
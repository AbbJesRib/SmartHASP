<template>
    <div>
        <virtual-list style="height: 360px; overflow-y: auto;"
        :data-key="'uid'"
        :data-sources="messageLog"
        :data-component="messageComponent"
        />
    </div>
</template>

<script lang="ts">

import message from './messageComponent.vue'
import Vue from 'vue';
import VirtualList from 'vue-virtual-scroll-list';

export default Vue.extend({
    name: 'logComponent',
    data() {
        return {
            messageComponent: message,
            messageLog: Array
        }
    },
    components: {'virtual-list': VirtualList}
})

</script>
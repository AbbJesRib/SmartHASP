<template>
    <div>
        <VirtualList style="height: 360px; overflow-y: auto;"
        :data-key="'uid'"
        :data-sources="messageLog"
        :data-component="messageComponent"
        />
    </div>
</template>

<script lang="ts">
import Vue, { PropType } from 'vue';
import VirtualList from 'vue-virtual-scroll-list';
import * as types from '../types'

export default Vue.extend({
    name: 'logComponent',
    props: {

    },
    data() {
        return {
            messageComponent: Object as PropType<types.Message>,
            messageLog: [],
        }
    },
})

</script>
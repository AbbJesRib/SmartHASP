<template>
    <div>
        <img v-if="this.gateOpen" src="../assets/closeButton.png" @click="switchButton" style="width:360px;">
        <img v-else src="../assets/openButton.png" @click="switchButton" style="width:360px;">
        <v-card>

        </v-card>
    </div>
</template>

<script lang="ts">

import Vue from 'vue';

export default Vue.extend({
    name: 'closeButton',
    props: {
        
    },
    data: () => ({
        gateOpen: true,
    }),
    methods: {
        sayHello() 
        {
            console.log("Hello world!");
        },
        switchButton() 
        {
            this.gateOpen = !this.gateOpen;
            if (this.gateOpen)
            {
                console.log('Opened gate!');
            }
            else 
            {
                console.log('Closed gate!');
            }
        },

    },
    mounted() 
    {

    },
});

</script>

<style scoped>

</style>
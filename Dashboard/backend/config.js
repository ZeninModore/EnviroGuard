import "dotenv/config";

const config = {
    port: Number(process.env.PORT) || 3000,

    esp32Url: process.env.ESP32_URL,

    apiKey: process.env.API_KEY,

    thresholds: {
        temperature: 35,
        humidity: 80,
        gas: 300
    },

    pollingInterval: 3000
};

export default config;
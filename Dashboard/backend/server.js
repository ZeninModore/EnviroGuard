import express from "express";
import cors from "cors";
import db from "./database.js";

import swaggerUi from "swagger-ui-express";
import openapiDocument from "./openapi.js";

import { processSensorData } from "./etl.js";
import config from "./config.js";

const app = express();


app.use(
    "/api-docs",
    swaggerUi.serve,
    swaggerUi.setup(openapiDocument)
);

app.use(cors());


const alertState = {
    motion: false,
    gas: false,
    temperature: false
};


function requireApiKey(req, res, next) {

    const apiKey = req.headers["x-api-key"];

    if (apiKey !== config.apiKey) {
        return res.status(401).json({
            error: "Invalid or missing API key"
        });
    }

    next();
}

// Get data from ESP32
async function getESP32Data() {
    const response = await fetch(
        `${config.esp32Url}/api/sensors`
    );

    if (!response.ok) {
        throw new Error(`ESP32 responded with status ${response.status}`);
    }

    return await response.json();
}

// Save sensor data into SQLite
function saveSensorReading(data) {
    const insertReading = db.prepare(`
        INSERT INTO sensor_readings
        (temperature, humidity, gas, motion, alarm)
        VALUES (?, ?, ?, ?, ?)
    `);

    insertReading.run(
        data.temperature,
        data.humidity,
        data.gas,
        data.motion ? 1 : 0,
        data.alarm ? 1 : 0
    );
}

function saveAlert(data) {

    const motionActive = Boolean(data.motion);

    const gasActive =
        data.gas >= config.thresholds.gas;

    const temperatureActive =
        data.temperature !== -999 &&
        data.temperature >=
            config.thresholds.temperature


    // Motion

    if (motionActive && !alertState.motion) {

        db.prepare(`
            INSERT INTO alerts (type, message)
            VALUES (?, ?)
        `).run(
            "motion",
            "Motion Detected: "
        );
    }

    alertState.motion = motionActive;


    // Gas

    if (gasActive && !alertState.gas) {

        db.prepare(`
            INSERT INTO alerts (type, message)
            VALUES (?, ?)
        `).run(
            "gas",
            "Gas Level High: "
        );
    }

    alertState.gas = gasActive;


    // Temperature

    if (
        temperatureActive &&
        !alertState.temperature
    ) {

        db.prepare(`
            INSERT INTO alerts (type, message)
            VALUES (?, ?)
        `).run(
            "temperature",
            "Temperature High: "
        );
    }

    alertState.temperature = temperatureActive;
}

// Get and save sensor data
async function updateSensorData() {
    try {

        const rawData = await getESP32Data();

        const cleanData =
            processSensorData(rawData);

        saveSensorReading(cleanData);
        saveAlert(cleanData);

        console.log(
            "Sensor data saved:",
            cleanData
        );

    } catch (error) {

        console.error(
            "Could not get data from ESP32:",
            error.message
        );
    }
}


// --------------------
// API endpoints
// --------------------

app.get("/", (req, res) => {
    res.json({
        message: "SmartLink IES backend is running"
    });
});

app.get(
    "/api/alerts",
    requireApiKey,
    (req, res) => {
    const alerts = db.prepare(`
        SELECT *
        FROM alerts
        ORDER BY id DESC
        LIMIT 20
    `).all();

    res.json(alerts);
});

app.get(
    "/api/alerts/active",
    requireApiKey,
    (req, res) => {
    const data = {
        motion: alertState.motion,
        gas: alertState.gas,
        temperature: alertState.temperature
    };

    const activeAlerts = [];

    if (data.motion) {
        activeAlerts.push({
            type: "motion",
            message: "Motion Detected"
        });
    }

    if (data.gas) {
        activeAlerts.push({
            type: "gas",
            message: "Gas Level High"
        });
    }

    if (data.temperature) {
        activeAlerts.push({
            type: "temperature",
            message: "Temperature High"
        });
    }

    res.json(activeAlerts);
});

app.get("/health", async (req, res) => {

    try {

        const response = await fetch(
            `${config.esp32Url}/health`
        );

        if (!response.ok) {
            throw new Error(
                `ESP32 responded with ${response.status}`
            );
        }

        const esp32Health = await response.json();

        res.json({
            status: "ok",
            backend: "online",
            esp32: esp32Health
        });

    } catch (error) {

        console.error(
            "Health check failed:",
            error.message
        );

        res.status(503).json({
            status: "degraded",
            backend: "online",
            esp32: "offline"
        });
    }
});

app.get(
    "/api/sensors/latest",
    requireApiKey,
    (req, res) => {
    const latestReading = db.prepare(`
        SELECT *
        FROM sensor_readings
        ORDER BY id DESC
        LIMIT 1
    `).get();

    res.json(latestReading);
});

app.get(
    "/api/readings",
    requireApiKey,
    (req, res) => {
    const readings = db.prepare(`
        SELECT *
        FROM sensor_readings
        ORDER BY id DESC
        LIMIT 20
    `).all();

    res.json(readings);
});


// Temporary test endpoint
app.get("/test/insert", (req, res) => {
    const insertReading = db.prepare(`
        INSERT INTO sensor_readings
        (temperature, humidity, gas, motion, alarm)
        VALUES (?, ?, ?, ?, ?)
    `);

    insertReading.run(28.5, 70.2, 123, 0, 0);

    res.json({
        message: "Test sensor reading inserted"
    });
});


// --------------------
// Start server
// --------------------

app.listen(config.port, () => {
    console.log(
        `Server running at http://localhost:${config.port}`
    );

    // Get first ESP32 reading immediately
    updateSensorData();

    // Then get a reading every 3 seconds
    setInterval(
        updateSensorData,
        config.pollingInterval
    );
});
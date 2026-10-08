const temperatureResult = document.querySelector(
    ".temperatureCelsiusResult"
);

const humidityResult = document.querySelector(
    ".humidityPercentageResult"
);

const gasResult = document.querySelector(
    ".gasPpmResult"
);

const motionResult = document.querySelector(
    ".motionStatusResult"
);

const systemStateText = document.querySelector(
    ".systemStateText"
);

const temperatureStatus = document.querySelector(
    ".temperatureStatusText"
);

const humidityStatus = document.querySelector(
    ".humidityStatusText"
);

const gasStatus = document.querySelector(
    ".gasStatusText"
);

const alertContainer = document.querySelector(
    ".alertInsertContainer"
);

const lastUpdatedText = document.querySelector(
    ".lastUpdatedText"
);

const dateText = document.querySelector(
    ".dateText"
);

const utcText = document.querySelector(
    ".utcText"
);


// ==================================================
// CHARTS
// ==================================================

const temperatureChart =
    document.querySelector("#temperatureChart");

const humidityChart =
    document.querySelector("#humidityChart");

const sensorChart =
    document.querySelector("#sensorChart");

const temperatureContext =
    temperatureChart.getContext("2d");

const humidityContext =
    humidityChart.getContext("2d");

const chartContext =
    sensorChart.getContext("2d");


// ==================================================
// OTHER ELEMENTS
// ==================================================

const alertHistoryList = document.querySelector(
    ".alertHistoryList"
);

const backendHealthText = document.querySelector(
    ".backendHealthText"
);

const esp32HealthText = document.querySelector(
    ".esp32HealthText"
);


// ==================================================
// CHART RESIZING
// ==================================================

function resizeChart() {

    const width =
        sensorChart.clientWidth;

    const height = 350;

    temperatureChart.width = width;
    temperatureChart.height = height;

    humidityChart.width = width;
    humidityChart.height = height;

    sensorChart.width = width;
    sensorChart.height = height;
}

resizeChart();


// ==================================================
// GENERIC SENSOR CHART
// ==================================================

function drawSensorChart(
    canvas,
    context,
    readings,
    valueKey,
    unit
) {

    context.clearRect(
        0,
        0,
        canvas.width,
        canvas.height
    );

    if (readings.length < 2) {
        return;
    }

    const width = canvas.width;
    const height = canvas.height;

    const padding = 60;

    const values = readings.map(
        reading => reading[valueKey]
    );


    // ----------------------------------------------
    // Find minimum and maximum
    // ----------------------------------------------

    const maxValue =
        Math.max(...values);

    const minValue =
        Math.min(...values);

    const range =
        maxValue - minValue || 1;


    // ----------------------------------------------
    // Grid lines
    // ----------------------------------------------

    const gridLines = 5;

    for (let i = 0; i <= gridLines; i++) {

        const y =
            height -
            padding -
            (i / gridLines) *
            (height - padding * 2);

        context.beginPath();

        context.moveTo(
            padding,
            y
        );

        context.lineTo(
            width - padding,
            y
        );

        context.stroke();


        // Y-axis value

        const value =
            minValue +
            (i / gridLines) *
            range;

        context.fillText(
            `${value.toFixed(1)} ${unit}`,
            10,
            y + 5
        );
    }


    // ----------------------------------------------
    // Sensor line
    // ----------------------------------------------

    context.beginPath();

    readings.forEach(
        (reading, index) => {

            const x =
                padding +
                (index /
                    (readings.length - 1)) *
                (width - padding * 2);


            const y =
                height -
                padding -
                ((reading[valueKey] -
                    minValue) /
                    range) *
                (height - padding * 2);


            if (index === 0) {

                context.moveTo(
                    x,
                    y
                );

            } else {

                context.lineTo(
                    x,
                    y
                );
            }
        }
    );

    context.stroke();


    // ----------------------------------------------
    // Data points
    // ----------------------------------------------

    readings.forEach(
        (reading, index) => {

            const x =
                padding +
                (index /
                    (readings.length - 1)) *
                (width - padding * 2);


            const y =
                height -
                padding -
                ((reading[valueKey] -
                    minValue) /
                    range) *
                (height - padding * 2);


            context.beginPath();

            context.arc(
                x,
                y,
                3,
                0,
                Math.PI * 2
            );

            context.fill();
        }
    );


    // ----------------------------------------------
    // X-axis time labels
    // ----------------------------------------------

    const firstReading =
        readings[0];

    const lastReading =
        readings[readings.length - 1];


    const firstTime =
        new Date(
            firstReading.created_at +
            " UTC"
        );


    const lastTime =
        new Date(
            lastReading.created_at +
            " UTC"
        );


    context.font =
        "12px Arial";


    context.fillText(
        firstTime.toLocaleTimeString(
            "en-US",
            {
                timeZone:
                    "Asia/Manila"
            }
        ),
        padding,
        height - 15
    );


    context.fillText(
        lastTime.toLocaleTimeString(
            "en-US",
            {
                timeZone:
                    "Asia/Manila"
            }
        ),
        width - padding - 70,
        height - 15
    );
}


// ==================================================
// UPDATE ALL CHARTS
// ==================================================

async function updateChart() {

    try {

        const response =
            await fetch(
                "http://localhost:3000/api/readings",
                {
                    headers: {
                        "x-api-key":
                            "enviroguard-demo-key"
                    }
                }
            );


        if (!response.ok) {

            throw new Error(
                `Server responded with ${response.status}`
            );
        }


        const readings =
            await response.json();


        // API returns newest first.
        // Reverse it so the chart goes
        // from oldest → newest.

        readings.reverse();


        // ------------------------------------------
        // Temperature chart
        // ------------------------------------------

        drawSensorChart(
            temperatureChart,
            temperatureContext,
            readings,
            "temperature",
            "°C"
        );


        // ------------------------------------------
        // Humidity chart
        // ------------------------------------------

        drawSensorChart(
            humidityChart,
            humidityContext,
            readings,
            "humidity",
            "%",
            "Humidity History"
        );


        // ------------------------------------------
        // Gas chart
        // ------------------------------------------

        drawSensorChart(
            sensorChart,
            chartContext,
            readings,
            "gas",
            "ppm",
            "Gas History"
        );

    } catch (error) {

        console.error(
            "Could not update charts:",
            error
        );
    }
}


// ==================================================
// ALERT CREATION
// ==================================================

function createAlert(message) {

    const alert =
        document.createElement("div");

    alert.classList.add(
        "alertItem"
    );

    alert.textContent =
        message;

    return alert;
}


// ==================================================
// ACTIVE ALERTS
// ==================================================

function updateAlerts(alerts) {

    alertContainer.innerHTML = "";


    if (alerts.length === 0) {

        alertContainer.appendChild(
            createAlert(
                "No Active Alerts"
            )
        );

        return;
    }


    alerts.forEach(alert => {

        alertContainer.appendChild(
            createAlert(
                alert.message
            )
        );
    });
}


// ==================================================
// ALERT HISTORY
// ==================================================

function displayAlertHistory(alerts) {

    alertHistoryList.innerHTML = "";


    if (alerts.length === 0) {

        alertHistoryList.innerHTML =
            "<p>No alerts recorded.</p>";

        return;
    }


    alerts.forEach(alert => {

        const alertElement =
            document.createElement(
                "div"
            );

        alertElement.classList.add(
            "alertHistoryItem"
        );


        const message =
            document.createElement(
                "span"
            );

        message.textContent =
            alert.message;


        const time =
            document.createElement(
                "span"
            );


        const alertDate =
            new Date(
                alert.created_at +
                " UTC"
            );


        time.textContent =
            alertDate.toLocaleString(
                "en-US",
                {
                    timeZone:
                        "Asia/Manila"
                }
            );


        alertElement.appendChild(
            message
        );

        alertElement.appendChild(
            time
        );


        alertHistoryList.appendChild(
            alertElement
        );
    });
}


// ==================================================
// UPDATE ALERT HISTORY
// ==================================================

async function updateAlertHistory() {

    try {

        const response =
            await fetch(
                "http://localhost:3000/api/alerts",
                {
                    headers: {
                        "x-api-key":
                            "enviroguard-demo-key"
                    }
                }
            );


        if (!response.ok) {

            throw new Error(
                `Server responded with ${response.status}`
            );
        }


        const alerts =
            await response.json();


        displayAlertHistory(
            alerts
        );

    } catch (error) {

        console.error(
            "Could not update alert history:",
            error
        );
    }
}


// ==================================================
// SYSTEM HEALTH
// ==================================================

async function updateSystemHealth() {

    try {

        const response =
            await fetch(
                "http://localhost:3000/health"
            );


        const data =
            await response.json();


        backendHealthText.textContent =
            data.backend
                .charAt(0)
                .toUpperCase() +
            data.backend.slice(1);


        if (
            data.esp32 ===
            "offline"
        ) {

            esp32HealthText.textContent =
                "Offline";

        } else {

            esp32HealthText.textContent =
                "Online";
        }

    } catch (error) {

        backendHealthText.textContent =
            "Offline";

        esp32HealthText.textContent =
            "Unknown";


        console.error(
            "Could not check system health:",
            error
        );
    }
}


// ==================================================
// UPDATE DASHBOARD
// ==================================================

async function updateDashboard() {

    try {

        const response =
            await fetch(
                "http://localhost:3000/api/sensors/latest",
                {
                    headers: {
                        "x-api-key":
                            "enviroguard-demo-key"
                    }
                }
            );


        if (!response.ok) {

            throw new Error(
                `Server responded with ${response.status}`
            );
        }


        const data =
            await response.json();


        // ------------------------------------------
        // Active alerts
        // ------------------------------------------

        const alertResponse =
            await fetch(
                "http://localhost:3000/api/alerts/active",
                {
                    headers: {
                        "x-api-key":
                            "enviroguard-demo-key"
                    }
                }
            );


        if (!alertResponse.ok) {

            throw new Error(
                `Alert server responded with ${alertResponse.status}`
            );
        }


        const alerts =
            await alertResponse.json();


        updateAlerts(
            alerts
        );


        // ------------------------------------------
        // Time information
        // ------------------------------------------

        const updatedDate =
            new Date(
                data.created_at +
                " UTC"
            );


        lastUpdatedText.textContent =
            `Last Updated: ${
                updatedDate.toLocaleTimeString(
                    "en-US",
                    {
                        timeZone:
                            "Asia/Manila"
                    }
                )
            }`;


        dateText.textContent =
            `Date: ${
                updatedDate.toLocaleDateString(
                    "en-US",
                    {
                        timeZone:
                            "Asia/Manila"
                    }
                )
            }`;


        utcText.textContent =
            "UTC: +8 (PST)";


        // ------------------------------------------
        // Sensor values
        // ------------------------------------------

        temperatureResult.textContent =
            `${data.temperature} °C`;


        humidityResult.textContent =
            `${data.humidity}%`;


        gasResult.textContent =
            `${data.gas} ppm`;


        motionResult.textContent =
            data.motion === 1
                ? "Motion Detected"
                : "No Motion";


        // ------------------------------------------
        // Temperature status
        // ------------------------------------------

        if (
            data.temperature === -999
        ) {

            temperatureStatus.textContent =
                "Unavailable";

        } else if (
            data.temperature >= 35
        ) {

            temperatureStatus.textContent =
                "High";

        } else {

            temperatureStatus.textContent =
                "Normal";
        }


        // ------------------------------------------
        // Humidity status
        // ------------------------------------------

        if (
            data.humidity === -999
        ) {

            humidityStatus.textContent =
                "Unavailable";

        } else if (
            data.humidity >= 80
        ) {

            humidityStatus.textContent =
                "High";

        } else {

            humidityStatus.textContent =
                "Normal";
        }


        // ------------------------------------------
        // Gas status
        // ------------------------------------------

        if (
            data.gas >= 300
        ) {

            gasStatus.textContent =
                "High";

        } else {

            gasStatus.textContent =
                "Normal";
        }

    } catch (error) {

        console.error(
            "Could not update dashboard:",
            error
        );
    }
}


// ==================================================
// WINDOW RESIZE
// ==================================================

window.addEventListener(
    "resize",
    () => {

        resizeChart();

        updateChart();
    }
);


// ==================================================
// INITIAL LOAD
// ==================================================

updateDashboard();

updateChart();

updateAlertHistory();

updateSystemHealth();


// ==================================================
// AUTOMATIC UPDATES
// ==================================================

setInterval(
    updateDashboard,
    3000
);

setInterval(
    updateChart,
    3000
);

setInterval(
    updateAlertHistory,
    3000
);

setInterval(
    updateSystemHealth,
    5000
);
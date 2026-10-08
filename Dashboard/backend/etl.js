function isValidNumber(value) {
    return typeof value === "number" &&
           Number.isFinite(value);
}

function transformSensorData(data) {

    const temperature =
        isValidNumber(data.temperature)
            ? data.temperature
            : -999;

    const humidity =
        isValidNumber(data.humidity)
            ? data.humidity
            : -999;

    const gas =
        isValidNumber(data.gas)
            ? data.gas
            : 0;

    const motion =
        Boolean(data.motion);

    const alarm =
        Boolean(data.alarm);

    return {
        temperature,
        humidity,
        gas,
        motion,
        alarm
    };
}

function validateSensorData(data) {

    if (
        data.temperature !== -999 &&
        (data.temperature < -40 ||
         data.temperature > 80)
    ) {
        return false;
    }

    if (
        data.humidity !== -999 &&
        (data.humidity < 0 ||
         data.humidity > 100)
    ) {
        return false;
    }

    if (data.gas < 0) {
        return false;
    }

    return true;
}

export function processSensorData(data) {

    const cleanData =
        transformSensorData(data);

    if (!validateSensorData(cleanData)) {
        throw new Error(
            "Invalid sensor data"
        );
    }

    return cleanData;
}
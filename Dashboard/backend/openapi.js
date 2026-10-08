const openapiDocument = {
    openapi: "3.0.0",

    info: {
        title: "EnviroGuard API",
        version: "1.0.0",
        description:
            "API for EnviroGuard: Smart Environmental Monitoring & Security System"
    },

    servers: [
        {
            url: "http://localhost:3000"
        }
    ],

    components: {
        securitySchemes: {
            ApiKeyAuth: {
                type: "apiKey",
                in: "header",
                name: "x-api-key"
            }
        }
    },

    paths: {

        "/health": {
            get: {
                summary: "Check system health",

                responses: {
                    "200": {
                        description:
                            "Backend and ESP32 health information"
                    },

                    "503": {
                        description:
                            "ESP32 or another system component is unavailable"
                    }
                }
            }
        },

        "/api/sensors/latest": {
            get: {
                summary: "Get the latest sensor reading",

                security: [
                    {
                        ApiKeyAuth: []
                    }
                ],

                responses: {
                    "200": {
                        description:
                            "Latest sensor reading"
                    },

                    "401": {
                        description:
                            "Missing or invalid API key"
                    }
                }
            }
        },

        "/api/readings": {
            get: {
                summary: "Get recent sensor readings",

                security: [
                    {
                        ApiKeyAuth: []
                    }
                ],

                responses: {
                    "200": {
                        description:
                            "List of recent sensor readings"
                    },

                    "401": {
                        description:
                            "Missing or invalid API key"
                    }
                }
            }
        },

        "/api/alerts": {
            get: {
                summary: "Get alert history",

                security: [
                    {
                        ApiKeyAuth: []
                    }
                ],

                responses: {
                    "200": {
                        description:
                            "List of recorded alerts"
                    },

                    "401": {
                        description:
                            "Missing or invalid API key"
                    }
                }
            }
        },

        "/api/alerts/active": {
            get: {
                summary: "Get currently active alerts",

                security: [
                    {
                        ApiKeyAuth: []
                    }
                ],

                responses: {
                    "200": {
                        description:
                            "List of currently active alerts"
                    },

                    "401": {
                        description:
                            "Missing or invalid API key"
                    }
                }
            }
        }
    }
};

export default openapiDocument;
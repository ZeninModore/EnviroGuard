import Database from "better-sqlite3";

const db = new Database("./data/enviroguard.db");

// Create sensor_readings table
db.prepare(`
    CREATE TABLE IF NOT EXISTS sensor_readings (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        temperature REAL,
        humidity REAL,
        gas INTEGER,
        motion INTEGER,
        alarm INTEGER,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    )
`).run();

// Create alerts table
db.prepare(`
    CREATE TABLE IF NOT EXISTS alerts (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        type TEXT,
        message TEXT,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    )
`).run();

console.log("Database connected.");

export default db;
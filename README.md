# 🏎️ Auto-Speed Language

> **Tagline:** You don’t *run* code — you *drive* it.

---

## 🚘 Overview

**Auto-Speed** is a **C-like programming language** themed entirely around **car racing concepts**.  
It replaces traditional programming keywords with fun and intuitive **racing terms**, letting you *drive* your logic instead of just running it.

---

## 🧠 Language Concept

| Concept | Auto-Speed Keyword | Example |
|----------|--------------------|----------|
| Program start | `ignite()` | `ignite() { ... }` |
| Function definition | `engine` | `engine turbo() { ... }` |
| Variable declaration | `gear`, `turbo`, `exhaust` | `gear fuel = 95;` |
| Output | `announce` | `announce "Race started!";` |
| Input | `listen` | `listen driverName;` |
| If | `track` | `track (speed > 100) { ... }` |
| Else | `pitstop` | `pitstop { ... }` |
| While | `looplap` | `looplap (fuel > 0) { ... }` |
| For | `overtake` | `overtake (gear i = 0; i < 5; i++) { ... }` |
| Return | `finishline` | `finishline 0;` |
| Include | `#oil` | `#oil <speed>` |
| Namespace | `key` | `key racetrack` |
| Comment | `#car` | `#car This is a comment` |

---

## 🧩 Data Types

| Type | Meaning | Example |
|------|----------|----------|
| `gear` | Integer value | `gear fuel = 95;` |
| `turbo` | Double value | `turbo Audi = 2.01;` |
| `exhaust` | String | `exhaust BMW = "AMG";` |
| `flag` | Boolean | `flag gameOn = true;` |

---

## 🏗️ Syntax Example

```auto-speed
#oil <speed>      #car importing main race library

key racetrack     #car declaring a namespace

engine fuelCheck(gear fuel) {
    track (fuel < 30) {
        announce "⚠️ Low fuel! Head to pitstop!";
    }
    pitstop {
        announce "✅ Fuel level is good.";
    }
    finishline 0;
}

engine raceStart() {
    gear lap = 0;
    gear fuel = 95;
    exhaust carName = "Ferrari";
    turbo speed = 2.5;
    flag engineOn = true;

    announce "🏁 Starting race with " + carName;
    announce "Engine turbo: " + speed;

    looplap (fuel > 0) {
        announce "Lap number: " + lap;
        lap = lap + 1;
        fuel = fuel - 10;
        speed = speed + 0.5;

        track (speed >= 3.0) {
            announce "🚀 Boost active!";
        }
    }

    announce "🏁 Race finished after " + lap + " laps.";
    announce "Remaining fuel: " + fuel;

    finishline fuel;
}

ignite() {
    announce "🏎️ Welcome to Auto-Speed!";
    listen driverName;
    announce "Driver: " + driverName;

    gear fuel = 95;
    fuelCheck(fuel);

    gear remaining = raceStart();
    track (remaining > 20) {
        announce "🏆 Great race, " + driverName + "!";
    }
    pitstop {
        announce "⛽ Time to refuel, " + driverName + "!";
    }
}

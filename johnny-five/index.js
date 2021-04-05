const { Board, Button, Servo } = require("johnny-five");
const board = new Board({
    port: "COM3"
});

let gearPositions = [
    10,
    20,
    30,
    40,
    50,
    60,
    70,
    80,
    90,
    100,
    110,
    120,
]

let currentGear = 1
let position = gearPositions[currentGear - 1];

function goto(position, servo, gear) {
    currentGear = currentGear + gear
    currentGear = currentGear < 1 ? 1 : currentGear > gearPositions.length ? gearPositions.length : currentGear;
    console.log(currentGear)
    if (position !== gearPositions[currentGear - 1]) {
        position = gearPositions[currentGear - 1]
        servo.to(position)
    }
}

board.on("ready", function () {
    const servo = new Servo(13);

    shiftDownButton = new Button(2);
    shiftUpButton = new Button(3)

    board.repl.inject({
        shiftDownButton,
        shiftUpButton,
        servo
    });

    // Button Event API

    shiftDownButton.on("down", function () {
        console.log("downshift");
        goto(position, servo, -1)
    });
    shiftUpButton.on("down", function () {
        console.log("upshift");
        goto(position, servo, 1)

    });
    shiftDownButton.on("hold", function () {
        console.log("downshift x3");
        goto(position, servo, -3)
    });
    shiftUpButton.on("hold", function () {
        console.log("upshift x3");
        goto(position, servo, 3)
    });
});
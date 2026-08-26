const logInput = document.getElementById("log-input");
const loadButton = document.getElementById("load-button");
const playButton = document.getElementById("play-button");
const restartButton = document.getElementById("restart-button");
const speedInput = document.getElementById("speed");
const speedValue = document.getElementById("speed-value");
const timeline = document.getElementById("timeline");

const simulationArea = document.getElementById("simulation-area");
const codersContainer = document.getElementById("coders-container");
const donglesContainer = document.getElementById("dongles-container");
const connections = document.getElementById("connections");

const currentTimeElement = document.getElementById("current-time");
const coderCountElement = document.getElementById("coder-count");
const eventCountElement = document.getElementById("event-count");
const compileCountElement = document.getElementById("compile-count");

const timelineTime = document.getElementById("timeline-time");
const currentEventElement = document.getElementById("current-event");

const coderList = document.getElementById("coder-list");
const eventLog = document.getElementById("event-log");

const statusDot = document.getElementById("status-dot");
const simulationStatus = document.getElementById("simulation-status");


let events = [];
let coderIds = [];
let coderState = {};

let currentEventIndex = 0;
let animationStart = null;
let playing = false;
let animationFrame = null;

let speed = 1;
let totalTime = 0;


/* ================================
   PARSE LOG
================================ */

function parseLog(text) {
    const lines = text
        .split("\n")
        .map(line => line.trim())
        .filter(line => line.length > 0);

    const parsedEvents = [];

    const regex =
        /^(\d+)\s+(\d+)\s+(has taken a dongle|is compiling|is debugging|is refactoring|burned out)$/;

    for (const line of lines) {
        const match = line.match(regex);

        if (!match)
            continue;

        parsedEvents.push({
            time: Number(match[1]),
            coder: Number(match[2]),
            action: match[3],
            raw: line
        });
    }

    parsedEvents.sort((a, b) => a.time - b.time);

    return parsedEvents;
}


/* ================================
   LOAD SIMULATION
================================ */

function loadSimulation() {
    stopSimulation();

    events = parseLog(logInput.value);

    if (events.length === 0) {
        alert("No valid Codexion events found.");
        return;
    }

    coderIds = [
        ...new Set(events.map(event => event.coder))
    ].sort((a, b) => a - b);

    totalTime = events[events.length - 1].time;

    initializeCoders();
    createCircularLayout();

    currentEventIndex = 0;

    timeline.min = 0;
    timeline.max = totalTime;
    timeline.value = 0;

    coderCountElement.textContent = coderIds.length;
    eventCountElement.textContent = events.length;

    updateSimulation(0);
    renderEventLog();

    setStatus("READY", "#64748b");
}


/* ================================
   INITIALIZE CODERS
================================ */

function initializeCoders() {
    coderState = {};

    for (const id of coderIds) {
        coderState[id] = {
            state: "WAITING",
            dongles: 0,
            compilations: 0,
            burned: false
        };
    }
}


/* ================================
   CIRCULAR LAYOUT
================================ */

function createCircularLayout() {
    codersContainer.innerHTML = "";
    donglesContainer.innerHTML = "";
    connections.innerHTML = "";

    const count = coderIds.length;

    const center = 50;
    const coderRadius = 40;
    const dongleRadius = 46;

    const positions = [];

    coderIds.forEach((id, index) => {
        const angle =
            (index / count) * Math.PI * 2 - Math.PI / 2;

        const x =
            center + Math.cos(angle) * coderRadius;

        const y =
            center + Math.sin(angle) * coderRadius;

        positions.push({ x, y });

        const coder = document.createElement("div");

        coder.className = "coder";
        coder.id = `coder-${id}`;

        coder.style.left = `${x}%`;
        coder.style.top = `${y}%`;

        coder.innerHTML = `
            <div class="coder-icon">👨‍💻</div>
            <div class="coder-name">CODER ${id}</div>
            <div class="coder-state">WAITING</div>
        `;

        codersContainer.appendChild(coder);
    });


    /*
        Create one dongle between each pair of coders.

        Dongle i is between coder i and coder i + 1.
    */

    for (let i = 0; i < count; i++) {

        const current = positions[i];
        const next = positions[(i + 1) % count];

        let x = (current.x + next.x) / 2;
        let y = (current.y + next.y) / 2;

        /*
            For the last-to-first connection,
            midpoint calculation requires a little
            correction because of circular wrapping.
        */

        if (Math.abs(current.x - next.x) > 50) {
            const angle =
                ((i + 0.5) / count) *
                Math.PI * 2 -
                Math.PI / 2;

            x =
                center +
                Math.cos(angle) * dongleRadius;

            y =
                center +
                Math.sin(angle) * dongleRadius;
        }

        const dongle = document.createElement("div");

        dongle.className = "dongle";
        dongle.id = `dongle-${i}`;

        dongle.style.left = `${x}%`;
        dongle.style.top = `${y}%`;

        dongle.textContent = "🔌";

        donglesContainer.appendChild(dongle);
    }

    createConnections(positions);
}


function createConnections(positions) {
    const count = positions.length;

    for (let i = 0; i < count; i++) {

        const start = positions[i];
        const end = positions[(i + 1) % count];

        let dx = end.x - start.x;
        let dy = end.y - start.y;

        if (Math.abs(dx) > 50 || Math.abs(dy) > 50)
            continue;

        const length =
            Math.sqrt(dx * dx + dy * dy);

        const angle =
            Math.atan2(dy, dx) * 180 / Math.PI;

        const line = document.createElement("div");

        line.className = "connection";

        line.style.left = `${start.x}%`;
        line.style.top = `${start.y}%`;
        line.style.width = `${length}%`;
        line.style.transform = `rotate(${angle}deg)`;

        connections.appendChild(line);
    }
}


/* ================================
   RESET STATE
================================ */

function resetState() {
    initializeCoders();

    updateCoderVisuals();
    updateDongles();

    currentEventElement.textContent =
        "Waiting for simulation...";
}


/* ================================
   UPDATE SIMULATION
================================ */

function updateSimulation(time) {
    resetState();

    let lastEvent = null;

    for (let i = 0; i < events.length; i++) {

        const event = events[i];

        if (event.time > time)
            break;

        applyEvent(event);

        lastEvent = event;
        currentEventIndex = i;
    }

    updateCoderVisuals();
    updateDongles();
    updateStatistics(time);

    if (lastEvent) {
        currentEventElement.textContent =
            `${lastEvent.time} ms — Coder ${lastEvent.coder} ${lastEvent.action}`;
    }

    highlightCurrentEvent();

    timeline.value = time;
}


/* ================================
   APPLY EVENT
================================ */

function applyEvent(event) {
    const coder = coderState[event.coder];

    if (!coder)
        return;

    switch (event.action) {

        case "has taken a dongle":
            coder.dongles++;
            coder.state = "WAITING";
            break;

        case "is compiling":
            coder.state = "COMPILING";
            coder.compilations++;
            break;

        case "is debugging":
            coder.state = "DEBUGGING";
            coder.dongles = 0;
            break;

        case "is refactoring":
            coder.state = "REFACTORING";
            break;

        case "burned out":
            coder.state = "BURNED OUT";
            coder.burned = true;
            coder.dongles = 0;
            break;
    }
}


/* ================================
   UPDATE CODERS
================================ */

function updateCoderVisuals() {
    for (const id of coderIds) {

        const coderElement =
            document.getElementById(`coder-${id}`);

        const state = coderState[id];

        coderElement.className = "coder";

        if (state.state === "COMPILING")
            coderElement.classList.add("coder-compile");

        if (state.state === "DEBUGGING")
            coderElement.classList.add("coder-debug");

        if (state.state === "REFACTORING")
            coderElement.classList.add("coder-refactor");

        if (state.state === "BURNED OUT")
            coderElement.classList.add("coder-burned");

        coderElement.querySelector(".coder-state").textContent =
            state.state;
    }

    renderCoderList();
}


/* ================================
   UPDATE DONGLes
================================ */

function updateDongles() {

    const count = coderIds.length;

    /*
        Approximation based on the logs:

        If a coder has taken dongles, highlight
        its left/right neighboring dongles.
    */

    for (let i = 0; i < count; i++) {
        const dongle =
            document.getElementById(`dongle-${i}`);

        dongle.classList.remove("used");
    }


    coderIds.forEach((id, index) => {

        const state = coderState[id];

        if (state.dongles === 0)
            return;

        const leftDongle =
            index;

        const rightDongle =
            (index - 1 + count) % count;

        const left =
            document.getElementById(
                `dongle-${leftDongle}`
            );

        const right =
            document.getElementById(
                `dongle-${rightDongle}`
            );

        if (state.dongles >= 1)
            left.classList.add("used");

        if (state.dongles >= 2)
            right.classList.add("used");
    });
}


/* ================================
   STATISTICS
================================ */

function updateStatistics(time) {

    currentTimeElement.textContent =
        `${Math.round(time)} ms`;

    timelineTime.textContent =
        `${Math.round(time)} / ${totalTime} ms`;

    let compilations = 0;

    coderIds.forEach(id => {
        compilations +=
            coderState[id].compilations;
    });

    compileCountElement.textContent =
        compilations;
}


/* ================================
   CODER LIST
================================ */

function renderCoderList() {
    coderList.innerHTML = "";

    coderIds.forEach(id => {

        const state = coderState[id];

        const item = document.createElement("div");

        item.className = "coder-list-item";

        const color = getStateColor(state.state);

        item.innerHTML = `
            <span>Coder ${id}</span>
            <strong
                class="state-badge"
                style="
                    color: ${color};
                    background: ${color}22;
                "
            >
                ${state.state}
            </strong>
        `;

        coderList.appendChild(item);
    });
}


function getStateColor(state) {

    switch (state) {

        case "COMPILING":
            return "#3b82f6";

        case "DEBUGGING":
            return "#22c55e";

        case "REFACTORING":
            return "#a855f7";

        case "BURNED OUT":
            return "#ef4444";

        default:
            return "#94a3b8";
    }
}


/* ================================
   EVENT LOG
================================ */

function renderEventLog() {

    eventLog.innerHTML = "";

    events.forEach((event, index) => {

        const element =
            document.createElement("div");

        element.className = "event-item";

        element.dataset.index = index;

        element.textContent = event.raw;

        eventLog.appendChild(element);
    });
}


function highlightCurrentEvent() {

    document
        .querySelectorAll(".event-item")
        .forEach(element => {
            element.classList.remove("active");
        });

    const active =
        document.querySelector(
            `.event-item[data-index="${currentEventIndex}"]`
        );

    if (active) {

        active.classList.add("active");

        active.scrollIntoView({
            block: "nearest",
            behavior: "smooth"
        });
    }
}


/* ================================
   PLAYBACK
================================ */

function playSimulation() {

    if (events.length === 0)
        return;

    playing = true;

    playButton.textContent = "⏸ Pause";

    setStatus("RUNNING", "#22c55e");

    const startTime =
        Number(timeline.value);

    animationStart =
        performance.now() -
        startTime / speed;

    animationFrame =
        requestAnimationFrame(animate);
}


function animate(now) {

    if (!playing)
        return;

    const simulationTime =
        (now - animationStart) * speed;

    if (simulationTime >= totalTime) {

        updateSimulation(totalTime);

        stopSimulation();

        setStatus("FINISHED", "#3b82f6");

        return;
    }

    updateSimulation(simulationTime);

    animationFrame =
        requestAnimationFrame(animate);
}


function stopSimulation() {

    playing = false;

    cancelAnimationFrame(animationFrame);

    playButton.textContent = "▶ Play";

    if (simulationStatus.textContent === "RUNNING")
        setStatus("PAUSED", "#f59e0b");
}


function restartSimulation() {

    stopSimulation();

    updateSimulation(0);

    setStatus("READY", "#64748b");
}


/* ================================
   STATUS
================================ */

function setStatus(text, color) {

    simulationStatus.textContent = text;

    statusDot.style.background = color;

    statusDot.style.boxShadow =
        `0 0 10px ${color}`;
}


/* ================================
   EVENT LISTENERS
================================ */

loadButton.addEventListener(
    "click",
    loadSimulation
);


playButton.addEventListener(
    "click",
    () => {

        if (playing)
            stopSimulation();
        else
            playSimulation();
    }
);


restartButton.addEventListener(
    "click",
    restartSimulation
);


speedInput.addEventListener(
    "input",
    () => {

        speed =
            Number(speedInput.value);

        speedValue.textContent =
            `${speed}x`;

        if (playing) {
            const current =
                Number(timeline.value);

            animationStart =
                performance.now() -
                current / speed;
        }
    }
);


timeline.addEventListener(
    "input",
    () => {

        stopSimulation();

        updateSimulation(
            Number(timeline.value)
        );

        setStatus("PAUSED", "#f59e0b");
    }
);


/* ================================
   INITIAL LOAD
================================ */

loadSimulation();
//------------------------------------------------------------------------------
// This is all the javascript needed for the graphing page to run.
// Contains scripts for the graph itself. It also contains the scripts
// for the UI to make it interactive.
//
// I opted for a more data/function oriented design because OOP gets 
// ridiculous very quickly. This tends to be much more effecient and
// easier to work with.
//
// I wanted to do this as a modular design across multiple files but
// apparently that is a huge pain with javascript

//------------------------------------------------------------------------------
// Graphics Implementation

const Styles = {
    dfltColor:  "rgb(0, 0, 0)",
    slcdColor: "rgb(7, 150, 0, 0.5)",
    vRadius: 25,
    lineWidth: 2,
    textWidth: 1

}

// Simple x,y coordinates
class Point {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }
}

//--------------------------------------
// Graphics Manager

class GraphicsManager {
    constructor() {
        this.vertices = [];
        this.dEdges = []
        this.uEdges = []
        this.slcdVertices = [];
        this.slcdEdges = [];
    }

    toJSON() {
        return {
            vertices: this.vertices,
            dEdges: this.dEdges,
            uEdges: this.uEdges
        }
    }
}

// Creates a new vertex at location with the current number of vertices as its
// label. The labels are done this way to make things simple/easy.
function graphic_newVertex(gm, location, label) {
    gm.vertices.push(new VertexGraphic(location, label));
}

function graphic_getVertexByLabel(gm, label) {
    return gm.vertices.find(v => {return v.label === label});
}

function graphic_getVertexByLocation(gm, point) {
    return gm.vertices.find(v => { return graphic_isInBounds(v, point)});
}

function graphic_selectVertex(gm, vertex) {
    vertex.color = Styles.slcdColor;
    gm.slcdVertices.push(vertex);
}

function graphic_deSelectVertex(gm, vertex) {
    vertex.color = Styles.dfltColor;
    gm.slcdVertices = gm.slcdVertices.filter(v => {return v != vertex});
}

// Creates a new directed edge graphic
function graphic_newDEdge(gm, vA, vB, weight) {
    gm.dEdges.push(new EdgeGraphic(vA, vB, weight));
}

// Creates a new undirected edge graphic
function graphic_newUEdge(gm, vA, vB, weight) {
    gm.uEdges.push(new EdgeGraphic(vA, vB, weight));
}

function graphic_getEdge(gm, vA, vB) {
    let edge = gm.uEdges.find(e => {
        return (e.vA == vA && e.vB == vB) || (e.vA == vB && e.vB == vA) });

    if(edge != undefined) {
        return edge;
    }

    edge = gm.dEdges.find(e => {
        return (e.vA == vA && e.vB == vB) || (e.vA == vB && e.vB == vA) });

    return edge;
}

function graphic_selectEdge(gm, edge) {
    edge.color = Styles.slcdColor;
    gm.slcdEdges.push(edge);
}

function graphic_deSelectEdge(gm, edge) {
    edge.color = Styles.dfltColor;
    gm.slcdEdges = gm.slcdEdges.filter(e => {return e != edge});
}

function graphic_draw(gm, cvs) {
    let ctx = cvs.getContext("2d");
    graphic_clearPage(cvs, ctx);
    graphic_drawGraphics(gm.vertices, ctx, graphic_drawVertex);
    graphic_drawGraphics(gm.uEdges, ctx, graphic_drawUEdge);
    graphic_drawGraphics(gm.dEdges, ctx, graphic_drawDEdge);
}

function graphic_clearPage(cvs, ctx) {
    ctx.clearRect(0, 0, cvs.width, cvs.height);
}

function graphic_drawGraphics(graphics, ctx, f) {
    graphics.forEach(i => f(i, ctx));
}

function graphic_clearSelected(gm) {
    gm.slcdVertices.forEach(v => {v.color = Styles.dfltColor});
    gm.slcdEdges.forEach(e => {e.color = Styles.dfltColor});

    gm.slcdVertices = [];
    gm.slcdEdges = [];
}

function graphic_clear(gm, cvs) {
    gm.vertices = [];
    gm.dEdges = []
    gm.uEdges = []
    gm.slcdVertices = [];
    gm.slcdEdges = [];
    graphic_draw(gm, cvs);
}

//--------------------------------------
// Vertex Graphic Implementation

// Graphical representation of a Vertex
class VertexGraphic {
    constructor(location, label) {
        this.location = location;
        this.label = label;
        this.color = Styles.dfltColor;
        this.lineWidth = Styles.lineWidth;
        this.textWidth = Styles.textWidth;
        this.radius = Styles.vRadius; // default value
    }

    toJSON() {
        return {
            label: this.label
        }
    }
}

//Draws a single vertex
function graphic_drawVertex(vertex, ctx) {
    ctx.strokeStyle = vertex.color;
    ctx.lineWidth = vertex.lineWidth;
    graphic_drawCircle(vertex.location, vertex.radius, ctx);

    ctx.lineWidth = vertex.textWidth;
    graphic_drawLabel(vertex.location, vertex.label, ctx);
}

// Returns true if point is inside the bounds of vertex
function graphic_isInBounds(vertex, point) {
    let xDiff = vertex.location.x - point.x;
    let yDiff = vertex.location.y - point.y;
    return Math.hypot(xDiff, yDiff) <= vertex.radius;
}

//--------------------------------------
// Edge Graphic Implemenation

class EdgeGraphic {
    constructor(vA, vB, weight) {
        this.vA = vA;
        this.vB = vB;
        this.weight = weight;
        this.color = Styles.dfltColor;
        this.lineWidth = Styles.lineWidth;
        this.textWidth = Styles.textWidth;
    }

    toJSON() {
        return {
            vA: this.vA.label,
            vB: this.vB.label,
            weight: this.weight
        }
    }
}

function graphic_drawUEdge(edge, ctx) {
    let pts = graphic_calcEdgePoints(edge.vA, edge.vB);
    ctx.strokeStyle = edge.color;
    ctx.lineWidth = edge.lineWidth;
    graphic_drawLine(pts.start, pts.end, ctx);

    let midPoint = graphic_calcMidPoint(edge.vA.location, edge.vB.location);
    ctx.lineWidth = edge.textWidth;
    graphic_drawLabel(midPoint, edge.weight, ctx);
}

function graphic_drawDEdge(edge, ctx) {
    graphic_drawUEdge(edge, ctx);

    ctx.fillStyle = edge.color;
    graphic_drawArrow(edge.vA, edge.vB, ctx);
}

//--------------------------------------
// Graphics Helpers

function graphic_calcMidPoint(pA, pB) {
    let x = (pA.x + pB.x) / 2;
    let y = (pA.y + pB.y) / 2;
    return new Point(x, y)
}

function graphic_calcEdgePoints(vA, vB) {
    let dx = vA.location.x - vB.location.x;
    let dy = vA.location.y - vB.location.y;
    let angle = Math.atan2(dy, dx);
    let x1 = (-vA.radius) * Math.cos(angle) + vA.location.x;
    let y1 = (-vA.radius) * Math.sin(angle) + vA.location.y;
    let x2 = vB.radius * Math.cos(angle) + vB.location.x;
    let y2 = vB.radius * Math.sin(angle) + vB.location.y;

    return { start: { x: x1, y: y1 }, end: { x: x2, y: y2 } };
}

function graphic_drawLine(pA, pB, ctx) {
    ctx.beginPath();
    ctx.moveTo(pA.x, pA.y);
    ctx.lineTo(pB.x, pB.y);
    ctx.stroke();
}

// draws the arrow to indicate direction
function graphic_drawArrow(vA, vB, ctx) {
    let headlen = 10;
    let dx = vB.location.x - vA.location.x;
    let dy = vB.location.y - vA.location.y;
    let angle = Math.atan2(dy, dx);
    let hx = 0;
    let hy = 0;
    let r = 20;
    let x = (-vB.radius) * Math.cos(angle) + vB.location.x;
    let y = (-vB.radius) * Math.sin(angle) + vB.location.y;
    ctx.beginPath()
    ctx.moveTo(x, y);
    angle += (1 / 3) * (2 * Math.PI)
    hx = r * Math.cos(angle) + x;
    hy = r * Math.sin(angle) + y;
    ctx.lineTo(hx, hy);
    angle += (1 / 3) * (2 * Math.PI)
    hx = r * Math.cos(angle) + x;
    hy = r * Math.sin(angle) + y;
    ctx.lineTo(hx, hy);
    ctx.fill();
    ctx.closePath();
}

// Draws a circle on a canvas
function graphic_drawCircle(center, radius, ctx) {
    ctx.beginPath();
    ctx.arc(center.x, center.y, radius, 0, 2 * Math.PI);
    ctx.stroke();
}

// Draws a label on a canvas
function graphic_drawLabel(location, label, ctx) {
    ctx.strokeText(label, location.x, location.y);
}


//------------------------------------------------------------------------------
// Graph Operations

function graph_isConnected(gm) {
    let visited = new Array(gm.vertices.length).fill(0);
    // Count number of edges connected to each vertex
    gm.uEdges.forEach(e => {
        visited[e.vA.label]++;
        visited[e.vB.label]++;
    });
    gm.dEdges.forEach(e => {
        visited[e.vA.label]++;
        visited[e.vB.label]++;
    });

    // if a vertex has no edge or only 1 vertex exists, returns true
    return visited.indexOf(0) == -1 || visited.length < 2;
}

function graph_isUndirected(gm) {
    // if graph has directed edge, returns false
    return gm.dEdges.length == 0 || gm.vertices.length < 2;
}

function graph_isPositive(gm) {
    let edge = gm.uEdges.find(e => {
        return e.weight < 0;
    });

    if(edge != undefined) {
        return false;
    }

    edge = gm.dEdges.find(e => {
        return e.weight < 0;
    });

    if(edge != undefined) {
        return false;
    }

    return true;
}

function graph_hasSufficientVertices(gm) {
    return gm.vertices.length > 1;
}

//------------------------------------------------------------------------------
// UI

// enum for possible modes
const Mode = {
    SELECT: 0,
    VERTEX: 1,
    U_EDGE: 2,
    D_EDGE: 3
}

const Algo = {
    PRIM: "prim"
}

class UIManager {
    constructor(gm, cvs) {
        this.mode = Mode.VERTEX;
        this.gm = gm;
        this.cvs = cvs;
        this.ctx = cvs.getContext("2d");
    }
}

// creates a new vertex graphic with center at point(x, y)
function ui_newVertex(ui, point) {
    graphic_newVertex(ui.gm, point, ui.gm.vertices.length);
    graphic_draw(ui.gm, ui.cvs);
}

function ui_validateInputs(gm, vA, vB, weight) {
    let isValid = false;
    if(weight == 0) {
        window.alert("Cannot have edge with weight 0");
    }
    else if(vA == vB) {
        window.alert("Cannot create loop");
    }
    else if(graphic_getEdge(gm, vA, vB) != undefined) {
        window.alert("Cannot create multiple edges between same vertices");
    }
    else {
        isValid = true;
    }
    return isValid;
}

// creates a new undirected edge graphic between v1 and v2
// does not allow multiple edges between same vertices or 
// loops.
function ui_newUEdge(ui, vA, vB, weight) {
    if(ui_validateInputs(ui.gm, vA, vB, weight)) {
        graphic_newUEdge(ui.gm, vA, vB, weight);
    }
    graphic_draw(ui.gm, ui.cvs);
}

// creates a new directed edge graphic from v1 to v2
// does not allow multiple edges between same vertices or 
// loops.
function ui_newDEdge(ui, vA, vB, weight) {
    if(ui_validateInputs(ui.gm, vA, vB, weight)) {
        graphic_newDEdge(ui.gm, vA, vB, weight);
    }
    graphic_draw(ui.gm, ui.cvs);
}

// returns graphical vertex within bounds of point(x,y)
function ui_getClickedVertex(ui, point) {
    return graphic_getVertexByLocation(ui.gm, point);
}

// Takes a point on the browser and returns a point on a canvas.
function ui_getPointOnCanvas(ui, point) {
    let rect = ui.cvs.getBoundingClientRect();
    let x = point.x - rect.left;
    let y = point.y - rect.top;
    return new Point(x, y);
}

function ui_reset(ui) {
    graphic_clear(ui.gm, ui.cvs);
    document.getElementById("weightCtrl").value = 1;
}

function ui_getEdgeWeight() {
    return document.getElementById("weightCtrl").value;
}

// Interactively builds an edge by selecting 2 vertices
function ui_buildEdge(ui, point) {
    let vGraphic = graphic_getVertexByLocation(ui.gm, point);

    if(vGraphic == undefined) {
        return
    }

    let nSelected = ui.gm.slcdVertices.length;
    if(nSelected == 0) {
        graphic_selectVertex(ui.gm, vGraphic);
    }
    else if(nSelected == 1) {
        let weight = ui_getEdgeWeight();
        if(ui.mode == Mode.U_EDGE) {
            ui_newUEdge(ui, ui.gm.slcdVertices[0], vGraphic, weight);
        }
        else if(ui.mode = Mode.D_EDGE) {
            ui_newDEdge(ui, ui.gm.slcdVertices[0], vGraphic, weight);
        }
        graphic_clearSelected(ui.gm);
    }
    graphic_draw(ui.gm, ui.cvs);
}

function ui_onClick(event) {
    let point = new Point(event.clientX, event.clientY);
    let canvasPoint = ui_getPointOnCanvas(userInterface, point);
    switch (userInterface.mode) {
        case Mode.VERTEX:
            ui_newVertex(userInterface, canvasPoint);
            break;
        case Mode.U_EDGE:
        case Mode.D_EDGE:
            ui_buildEdge(userInterface, canvasPoint);
            break;
        default:
    }
}

function ui_toVertexMode(ui) {
    ui.mode = Mode.VERTEX;
    graphic_clearSelected(ui.gm)
}

function ui_setMode(ui, mode) {
    ui.mode = mode;
    graphic_clearSelected(ui.gm)
}

function ui_satisfiesAlgoConditions(gm, algo) {
    if(algo == "prim" && graph_isPositive(gm) &&
      graph_isConnected(gm) &&
      graph_isUndirected(gm) &&
      graph_hasSufficientVertices(gm)) {
        return true;
    }
    else if((algo == "bfs" || algo == "dfs") && graph_isPositive(gm) &&
      graph_hasSufficientVertices(gm) &&
      graph_isConnected(gm)) {
          return true;
      }
    else {
        return false;
    }
}

function ui_getAlgorithm() {
    return document.getElementById("selectAlgo").value;
}

function ui_requestCalculation(ui) {
    let algo = ui_getAlgorithm();
    if(ui_satisfiesAlgoConditions(ui.gm, algo)) {
        com_requestCalculation(ui, algo);
    }
    else {
        window.alert("The graph does not satisfy algorithm conditions");
    }
}

function ui_displayResult(ui, result) {
    let vA = undefined;
    let vB = undefined;
    let edge = undefined;
    graphic_clearSelected(ui.gm);


    result.uEdges.forEach(e => {
        vA = graphic_getVertexByLabel(ui.gm, e.vA);
        vB = graphic_getVertexByLabel(ui.gm, e.vB);
        edge = graphic_getEdge(ui.gm, vA, vB);
        graphic_selectVertex(ui.gm, vA);
        graphic_selectVertex(ui.gm, vB);
        graphic_selectEdge(ui.gm, edge)});

    result.dEdges.forEach(e => {
        vA = graphic_getVertexByLabel(ui.gm, e.vA);
        vB = graphic_getVertexByLabel(ui.gm, e.vB);
        edge = graphic_getEdge(ui.gm, vA, vB);
        graphic_selectVertex(ui.gm, vA);
        graphic_selectVertex(ui.gm, vB);
        graphic_selectEdge(ui.gm, edge)});

    graphic_draw(ui.gm, ui.cvs);
}

// resizes the canvas when the window is resized
function ui_resizeCanvas(cvs) {
    cvs.width = window.innerWidth;
    cvs.height = window.innerHeight;
    graphic_draw(userInterface.gm, userInterface.cvs);
}

var userInterface = new UIManager(new GraphicsManager(), document.getElementById("canvas"));

// event listener for clicking on the canvas
// checks current mode and acts accordingly
userInterface.cvs.addEventListener("click", ui_onClick);

// add listener for reset button
document.getElementById("resetBtn").addEventListener("click", function() {ui_reset(userInterface)});

// add listener for vertex button
document.getElementById("vertBtn").addEventListener("click", function() { ui_setMode(userInterface, Mode.VERTEX)});

 // add listener for undirected edge button
document.getElementById("uedgeBtn").addEventListener("click", function() {ui_setMode(userInterface, Mode.U_EDGE)});

// add listener for directed edge button
document.getElementById("dEdgeBtn").addEventListener("click", function () {ui_setMode(userInterface, Mode.D_EDGE)});

document.getElementById("weightCtrl").value = 1;

// add listener foir select mode button. This may get removed
document.getElementById("executeBtn").addEventListener("click", function() {
    ui_requestCalculation(userInterface, ui_getAlgorithm());
});

window.addEventListener('resize', function() {
    ui_resizeCanvas(userInterface.cvs);
});
//------------------------------------------------------------------------------
// Server Communication

function com_recieve(ui, response) {
    let json = JSON.parse(response);
    ui_displayResult(ui, json);
}

function com_requestCalculation(ui, algorithm) {
    //var url = "calculate?algo=" + algorithm + "&graph=" + encodeURIComponent(JSON.stringify(ui.graph));
    var url = "calculate?algo=" + algorithm + "&graph=" + JSON.stringify(ui.gm);
    var request = new XMLHttpRequest();
    request.open("GET", url, true);
    // request.setRequestHeader("Content-Type", "application/json");
    request.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            com_recieve(ui, request.responseText);
        }
    };
    //console.log(url);
    request.send();
}

ui_resizeCanvas(userInterface.cvs);
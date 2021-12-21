var VertexNode = /** @class */ (function () {
    function VertexNode() {
    }
    return VertexNode;
}());
var Vertex = /** @class */ (function () {
    function Vertex(theName, theNodes, theWeight) {
        if (theWeight === void 0) { theWeight = 1; }
        this.name = theName;
        this.nodes = theNodes;
        this.weight = theWeight;
    }
    return Vertex;
}());
var Dijkstra = /** @class */ (function () {
    function Dijkstra() {
        this.vertices = {};
    }
    Dijkstra.prototype.addVertex = function (vertex) {
        this.vertices[vertex.name] = vertex;
    };
    Dijkstra.prototype.setPathToNodes = function (nodeName, weight) {
        return { nameOfVertex: nodeName.toUpperCase(), weight: weight };
    };
    Dijkstra.prototype.findPointsOfShortestWay = function (start, finish, weight) {
        var nextVertex = finish;
        var arrayWithVertex = [];
        while (nextVertex !== start) {
            var minWeigth = Number.MAX_VALUE;
            var minVertex = "";
            for (var _i = 0, _a = this.vertices[nextVertex].nodes; _i < _a.length; _i++) {
                var i = _a[_i];
                if (i.weight + this.vertices[i.nameOfVertex].weight < minWeigth) {
                    minWeigth = this.vertices[i.nameOfVertex].weight;
                    minVertex = i.nameOfVertex;
                }
            }
            arrayWithVertex.push(minVertex);
            nextVertex = minVertex;
        }
        return arrayWithVertex;
    };
    Dijkstra.prototype.findShortestWay = function (start, finish) {
        var _this = this;
        var nodes = {};
        var visitedVertex = [];
        for (var i in this.vertices) {
            if (this.vertices[i].name === start) {
                this.vertices[i].weight = 0;
            }
            else {
                this.vertices[i].weight = Number.MAX_VALUE;
            }
            nodes[this.vertices[i].name] = this.vertices[i].weight;
        }
        while (Object.keys(nodes).length !== 0) {
            var sortedVisitedByWeight = Object.keys(nodes).sort(function (a, b) { return _this.vertices[a].weight - _this.vertices[b].weight; });
            var currentVertex = this.vertices[sortedVisitedByWeight[0]];
            for (var _i = 0, _a = currentVertex.nodes; _i < _a.length; _i++) {
                var j = _a[_i];
                var calculateWeight = currentVertex.weight + j.weight;
                if (calculateWeight < this.vertices[j.nameOfVertex].weight) {
                    this.vertices[j.nameOfVertex].weight = calculateWeight;
                }
            }
            delete nodes[sortedVisitedByWeight[0]];
        }
        var finishWeight = this.vertices[finish].weight;
        var arrayWithVertex = this.findPointsOfShortestWay(start, finish, finishWeight).reverse();
        arrayWithVertex.push(finish, finishWeight.toString());
        return arrayWithVertex;
    };
    return Dijkstra;
}());
var graph = new Dijkstra();
graph.addVertex(new Vertex("A", [
    graph.setPathToNodes("B", 3),
    graph.setPathToNodes("C", 5)
]));
graph.addVertex(new Vertex("B", [
    graph.setPathToNodes("A", 3),
    graph.setPathToNodes("C", 2),
    graph.setPathToNodes("D", 6)
]));
graph.addVertex(new Vertex("C", [
    graph.setPathToNodes("B", 2),
    graph.setPathToNodes("A", 5),
    graph.setPathToNodes("d", 5)
]));
graph.addVertex(new Vertex("D", [
    graph.setPathToNodes("B", 6),
    graph.setPathToNodes("C", 5)
]));
console.log(graph.findShortestWay("A", "D"));

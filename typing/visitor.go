package main
import "fmt"

// ориентированный граф
type Graph map[string][]string

// интерфейс Visiter'а
type IVertexCallback interface {
	discover_vertex( vertex string )
	finish_vertex( vertex string )
}

// точка входа
func main() {
	graph := createGraph()
	var callback VertexPrinter

	dfs( graph, "C", callback )
}

// функция создания графа
func createGraph() Graph {
	return map[string][]string {
		"A": []string{ "B", "C" },
		"B": []string{ "B", "C" },
		"C": []string{ "A", "F" },
		"D": []string{ "B" },
		"E": []string{ "B", "F" },
		"F": []string{ "C", "E" } }
}

// обход графа
func dfs( graph Graph, start string, callback IVertexCallback, _visited ...map[string]bool ) {

	// "set" из посещенных вершин
	var visited map[string]bool;

	// в Go нет аргументов по-умолчанию
	if len(_visited) > 0 {
		visited = _visited[0]
	} else {
		visited = make(map[string]bool)
	}

	visited[start] = true

	callback.discover_vertex( start )
	for _, vertex := range graph[start] {
		if !visited[vertex] {
			dfs(graph, vertex, callback, visited)
		}
	}

	callback.finish_vertex( start )
}

// конкретный visiter. Не задаём явно его интерфейс
type VertexPrinter struct{
}

// но реализуем интерфейс IVertexCallback
func (printer VertexPrinter) discover_vertex( vertex string ) {
	fmt.Print( vertex )
	fmt.Print( " -> " )
}

func (printer VertexPrinter) finish_vertex( vertex string ) {
}
// конец реализации интерфейса

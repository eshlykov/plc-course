data class Cell(var content: Any? = null)
data class Row(var cells: MutableList<Cell> = arrayListOf())
data class Table(var rows: MutableList<Row> = arrayListOf())

fun table ( lambda : Table.() -> Unit ) : Table = Table().apply(lambda)

fun Table.tr( lambda : Row.() -> Unit ) {
    val row = Row()
    row.lambda()
    this.rows.add(row)
}

fun Row.td( lambda : Cell.() -> Unit ) {
    val cell = Cell()
    cell.lambda()
    this.cells.add(cell)
}

fun main(args: Array<String>) {
    // Реализовать такое
    println(table {
        for (i in 1..2) {
            tr {
                td {
                }
                td {
                }
            }
        }
    })
}

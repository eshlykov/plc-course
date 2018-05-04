package dsl.ex05

fun <T> T.myApply(lambda: T.() -> Unit) = this.lambda()

fun <T> myWith(obj: T, lambda : T.() -> Unit) { obj.lambda() }

fun <T> T.myApplyChain( lambda : T.() -> Unit ) : T {
    this.lambda()
    return this
}

fun main(args: Array<String>) {
    val map = mutableMapOf(1 to "one")
    map.apply { this[2] = "two"}
    with (map) { this[3] = "three" }
    println(map)

    // Есть стандартные функции apply и with. Нужно написать их реализации самостоятельно
    map.myApply { this[4] = "four"}
    myWith (map) { this[5] = "five" }
    println(map)

    map.myApplyChain { this[6] = "six"} .myApplyChain { this[7] = "seven" }
    println(map)
}

package dsl.ex03
// Напишите extension-method IsNullOrSingleLetter для строки

fun String?.IsNullOrSingleLetter() = this == null || this.length == 1

fun main(args: Array<String>) {
    val line : String? = null
    println(line.IsNullOrSingleLetter())
    println("A".IsNullOrSingleLetter())
    println("Hello, world".IsNullOrSingleLetter())
}

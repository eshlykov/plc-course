package dsl.ex02

fun foo(line : String, transform : (String) -> String) = transform(line)

// lambda with a receiver
fun fooReceiver(line : String, transform : String.() -> String) = transform(line)

fun main(args: Array<String>) {
    println(foo("Hello, world", {it.substring(0, 5)}))
    println(foo("Hello, world") {it.substring(0, 5)})
    println(fooReceiver("Hello, world") {substring(0, 5)})
}

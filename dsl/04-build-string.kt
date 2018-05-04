package dsl.ex04

fun buildString(builderAction : (StringBuilder) -> Unit) : String {
    val sb = StringBuilder()
    builderAction(sb)
    return sb.toString()
}

fun buildStringReciever(builderAction : StringBuilder.() -> Unit) : String {
    val sb = StringBuilder()
    sb.builderAction()
    return sb.toString()
}

fun main(args: Array<String>) {
    val stringBuilder = StringBuilder()
    // 1. Добавьте в stringBuilder все латинские буквы от A до Z
    for (i in 'A'..'Z') {
        stringBuilder.append(i)
    }
    val line = stringBuilder.toString()
    println(line)

    // 2. Воспользуйтесь buildString для того же
    println(buildString {
        for (i in 'A'..'Z') {
            it.append(i)
      }
    })

    // 3. Переделаем всё с помощью lambda with a receiver
    println(buildStringReciever {
        for (i in 'A'..'Z') {
            append(i)
        }
    })
}

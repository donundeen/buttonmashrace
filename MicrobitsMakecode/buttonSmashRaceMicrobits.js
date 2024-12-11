function resetGame () {
    p2score = 0
    p1score = 0
    playing = true
    strip.showColor(neopixel.colors(NeoPixelColors.Red))
    strip.show()
}
function showWinner (text: string) {
    playing = false
    for (let index = 0; index < 10; index++) {
        strip.showColor(neopixel.colors(NeoPixelColors.Red))
        strip.show()
        basic.pause(200)
        if (text == "p1") {
            strip.showColor(neopixel.colors(NeoPixelColors.Blue))
        }
        if (text == "p2") {
            strip.showColor(neopixel.colors(NeoPixelColors.Green))
        }
        strip.show()
        basic.pause(200)
    }
    for (let index = 0; index <= 9; index++) {
        strip.setPixelColor(9 - index, neopixel.colors(NeoPixelColors.Red))
        strip.setPixelColor(9 + (1 + index), neopixel.colors(NeoPixelColors.Red))
        strip.show()
        basic.pause(500)
    }
    resetGame()
}
input.onPinPressed(TouchPin.P2, function () {
    if (playing) {
        p2score = p2score + 1
        if (p2score + p1score > 20) {
            p1score = p1score - (p2score + p1score - 20)
        }
        updateStrip()
    }
})
function updateStrip () {
    for (let index = 0; index <= p1score; index++) {
        strip.setPixelColor(index - 1, neopixel.colors(NeoPixelColors.Blue))
    }
    for (let index = 0; index <= p2score; index++) {
        strip.setPixelColor(20 - index, neopixel.colors(NeoPixelColors.Green))
    }
    strip.show()
    if (p1score > 20) {
        showWinner("p1")
    }
    if (p2score > 20) {
        showWinner("p2")
    }
}
input.onPinPressed(TouchPin.P1, function () {
    if (playing) {
        p1score = p1score + 1
        if (p1score + p2score > 20) {
            p2score = p2score - (p1score + p2score - 20)
        }
        updateStrip()
    }
})
let playing = false
let p1score = 0
let p2score = 0
let strip: neopixel.Strip = null
strip = neopixel.create(DigitalPin.P0, 20, NeoPixelMode.RGB)
resetGame()

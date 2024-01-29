from signalflow import *
graph = AudioGraph()

#-------------------------------------------------------------------------------
# Simple square wave oscillator shaped by an envelope
#-------------------------------------------------------------------------------
square = SquareOscillator(440)
envelope = ASREnvelope(0, 0.1, 0.5)
output = square * envelope
output.play()
graph.wait()
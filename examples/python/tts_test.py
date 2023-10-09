"""Example of how the C++ examples would be implemented directly in Python.
"""
import sys
import numpy
import sounddevice as sd

if __name__ == "__main__":
    print("Initializing TTS...")
    from TTS.api import TTS

    print("Available Models:")
    for num, model in enumerate(TTS.list_models()):
        print(f"{num}: {model}")

    tts = TTS(TTS.list_models()[10])

    data = tts.tts("This is a TTS test!")
    sample_rate = tts.synthesizer.output_sample_rate

    sd.play(data, sample_rate, blocking=True)

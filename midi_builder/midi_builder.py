#!/usr/bin/python3
import mido


if __name__ == '__main__':
    t = 0
    freqs =[]
    starts = []
    stops = []

    for msg in mido.MidiFile('t4vnju.midi'):
        if not msg.is_meta and msg.channel == 0 and msg.type == "note_on":
            t = t + msg.time
            note = msg.note
            freq = round(27.5 * 2 ** ((note - 21) / 12))
            velocity = msg.velocity
            print(t, freq, msg)

            if velocity == 0:
                stops.append(t)
            else:
                starts.append(t)
                freqs.append(freq)

    assert(len(freqs) == len(starts) == len(stops))

    print(freqs)
    print(starts)
    print(stops)

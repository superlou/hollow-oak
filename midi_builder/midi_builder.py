#!/usr/bin/python3
import mido
import textwrap

def array_to_str(datatype, name, values):
    text = "{} {}[] = ".format(datatype, name)
    text += "{\n"
    text += textwrap.fill(", ".join([str(value) for value in values]),
                          initial_indent="  ", subsequent_indent="  ")
    text += "\n};"

    return text


def build_midi(filename):
    t = 0
    freqs =[]
    starts = []
    stops = []

    for msg in mido.MidiFile(filename):
        if not msg.is_meta and msg.channel == 0 and msg.type == "note_on":
            t = t + msg.time
            note = msg.note
            freq = round(27.5 * 2 ** ((note - 21) / 12))
            velocity = msg.velocity
            print(t, freq, msg)

            if velocity == 0:
                stops.append(int(t * 1000))
            else:
                starts.append(int(t * 1000))
                freqs.append(freq)

    assert(len(freqs) == len(starts) == len(stops))

    print(array_to_str("int", "s1_freqs", freqs))
    print(array_to_str("int", "s1_starts", starts))
    print(array_to_str("int", "s1_stops", stops))


if __name__ == '__main__':
    build_midi("spider.mid")

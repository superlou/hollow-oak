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


def build_midi(filename, name):
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
            # print(t, freq, msg)

            if velocity == 0:
                stops.append(int(t * 1000))
            else:
                starts.append(int(t * 1000))
                freqs.append(freq)

    assert(len(freqs) == len(starts) == len(stops))

    t0 = starts[0]
    starts = [time - t0 for time in starts]
    stops = [time - t0 for time in stops]

    print(array_to_str("int", "{}_freqs".format(name), freqs))
    print(array_to_str("int", "{}_starts".format(name), starts))
    print(array_to_str("int", "{}_stops".format(name), stops))


if __name__ == '__main__':
    build_midi("spider1.mid", "spider1")
    build_midi("spider2.mid", "spider2")
    build_midi("spider3.mid", "spider3")
    build_midi("spider4.mid", "spider4")
    build_midi("spider5.mid", "spider5")

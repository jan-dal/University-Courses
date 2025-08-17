lata, latb = '\u0000', '\u007f'
grka, grkb = '\u0370', '\u03ff'
cyra, cyrb = '\u0400', '\u04ff'

s = "οОсуοоауееaоeΙеceасеeyοоеKоcacpοсаеоοеοpοpoοеΡаοaсoaеоCοcοyΙOеесΒaеТусоаоyNооocааеоeyeаeeсοoοароcаcеyoрaсceapoΤooреΟaоayaрoοеpοеΙaaοepеοοΤycypοΙаcоeусepοoаВоTaapoсceаΟcyуooΤycaеpеоooοуcpaeycoyсеοуceоoayΝееoсoусоусpаоаyсоеοaeyaсаyсреcpоеaycyeеуοуeорyaaeaаеpaeесeсеapaaAуοpеpaaeаyеΖeοaеaaсcеeеаcpрeаyаеecаNарaеееоoуaааcecеaеcpοοауесyаеaроеаесаyТуеeeοeoceeееTοpоcοоοoаееoeaуееeceсоΙοοаepyeyοсoрοeсecеоeaеoосaоаеοeIaсуοpcсοeеcоаayаcypΟсуссeeаyооeοCаοeеарeеaeеoοpаeοeaaоοοуceeyoоaееаpоеВaaacоааpесooоааеoсаусосaрaaaеοyecaоураeΙοeyеeсаcрeаeyaрοoоерοcoeаеcурсacуроοpοa"

#with open("pan_t.txt", "r") as f:
#    s = f.read()


def v(x):
    if lata <= x <= latb:
        return 0
    elif cyra <= x <= cyrb:
        return 1
    elif grka <= x <= grkb:
        return 2
    print(f"Not supported: {x}")
    return 0

def b(x):
    if lata <= x <= latb:
        return 1
    elif cyra <= x <= cyrb:
        return 2
    elif grka <= x <= grkb:
        return 3
    print(f"Not supported: {x}")
    return 0

def decode(s):
    num = 0
    base = 1
    for c in s:
        num += base * v(c)
        base *= b(c)
    return num


print(decode(s))

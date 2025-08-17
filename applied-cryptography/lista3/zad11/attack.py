from hashlib import sha256


txt = """Nieśmiały cybernetyk potężne ekstrema{}
Poznawał, kiedy grupy unimodularne{}
Cyberiady całkował w popołudnie parne,{}
Nie wiedząc, czy jest miłość, czy jeszcze jej nie ma.{}
{}
Precz mi, precz, Laplasjany z wieczora do ranka,{}
I wersory wektorów z ranka do wieczora!{}
Bliżej, przeciwobrazy! Bliżej, bo już pora{}
Zredukować kochankę do objęć kochanka!{}
{}
On drżenia wpółmetryczne, które jęk jednoczy,{}
Zmieni w grupy obrotów i sprzężenia zwrotne,{}
A takie kaskadowe, a takie zawrotne,{}
Że zwarciem zagrażają, idąc z oczu w oczy!{}
{}
Ty, klaso transfinalna! Ty, silna wielkości!{}
Nieprzywiedlne continuum! Praukładzie biały!{}
Christoffela ze Stoksem oddam na wiek cały{}
Za pierwszą i ostatnią pochodną miłości.{}
{}
Twych skalarnych przestrzeni wielolistne głębie{}
Ukaż uwikłanemu w Teoremat Ciała,{}
Cyberiado cyprysów, bimodalnie cała{}
W gradientach, rozmnożonych na loty gołębie!{}
{}
O, nie dożył rozkoszy, kto tak bez siwizny{}
Ani w przestrzeni Weyla, ani Brouwera{}
Studium topologiczne uściskiem otwiera,{}
Badając Moebiusowi nie znane krzywizny!{}
{}
O, wielopowłokowa uczuć komitanto,
Wiele trzeba cię cenić, ten się dowie tylko,
Kto takich parametrów przeczuwając fantom,
Ginie w nanosekundach, płonąc każdą chwilką!

Jak punkt, wchodzący w układ holonomiczności,
Pozbawiany współrzędnych zera asymptotą,
Tak w ostatniej projekcji, ostatnią pieszczotą
Żegnany — cybernetyk umiera z miłości.
"""
spaces = ["" for i in range(30)]

for i in range(1 << 30):
    t = i
    for j in range(30):
        if (t & 1 == 1):
            spaces[j] = " "
        else:
            spaces[j] = ""
        t >>= 1

    prefix = sha256(txt.format(*spaces).encode()).hexdigest()[:6]

    if (prefix == "000000"):
        print(f"Found: {i}")
        with open("result.txt", "w") as w:
            w.write(txt.format(*spaces))
        break
    if (i % 1000000 == 0):
        print(i)


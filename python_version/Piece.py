
class Piece:
    def __init__(self, value, identifier):
        self.value = value
        # UP    : 0x01
        # DOWN  : 0x02
        # LEFT  : 0x04
        # RIGHT : 0x08
        self.up    = (value & 0x1) != 0
        self.down  = (value & 0x2) != 0
        self.left  = (value & 0x4) != 0
        self.right = (value & 0x8) != 0

        self.identifier = identifier # String identifier

    def __repr__(self):
        return self.identifier


def create_pieces():
    # Create all piece elements
    pieces = []
            # UP    : 0x01
            # DOWN  : 0x02
            # LEFT  : 0x04
            # RIGHT : 0x08
    pieces.append(Piece(0, "."))
    pieces.append(Piece(1, "^"))
    pieces.append(Piece(2, "v"))
    pieces.append(Piece(3, "|"))
    pieces.append(Piece(4, "<"))
    pieces.append(Piece(5, u'\u2518'))   #  0xD9 -> BOX DRAWINGS LIGHT UP AND LEFT
    pieces.append(Piece(6, u'\u2510'))   #  0xBF -> BOX DRAWINGS LIGHT DOWN AND LEFT
    pieces.append(Piece(7, u'\u2524'))   #  0xB4 -> BOX DRAWINGS LIGHT VERTICAL AND LEFT
    pieces.append(Piece(8, ">"))
    pieces.append(Piece(9, u'\u2514'))   #  0xC0 -> BOX DRAWINGS LIGHT UP AND RIGHT
    pieces.append(Piece(10,u'\u250c'))   #  0xDA -> BOX DRAWINGS LIGHT DOWN AND RIGHT
    pieces.append(Piece(11,u'\u251c'))   #  0xC3 -> BOX DRAWINGS LIGHT VERTICAL AND RIGHT
    pieces.append(Piece(12,u'\u2500'))   #  0xC4 -> BOX DRAWINGS LIGHT HORIZONTAL
    pieces.append(Piece(13,u'\u2534'))   #  0xC1 -> BOX DRAWINGS LIGHT UP AND HORIZONTAL
    pieces.append(Piece(14,u'\u252c'))   #  0xC2 -> BOX DRAWINGS LIGHT DOWN AND HORIZONTAL
    pieces.append(Piece(15,u'\u253c'))   #  0xC5 -> BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL

    return pieces

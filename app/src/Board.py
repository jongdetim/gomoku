# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Board.py                                           :+:    :+:             #
#                                                      +:+                     #
#    By: fhignett <fhignett@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2021/09/07 17:53:46 by fhignett      #+#    #+#                  #
#    Updated: 2021/09/07 19:34:56 by fhignett      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

import numpy as np

BOARDSIZE = 19

class Board:
    def __init__(self):
        self.board = np.zeros((BOARDSIZE, BOARDSIZE), dtype=np.uint8)

    def __str__(self):
        return np.array2string(self.board)

    def __setitem__(self, pos, value):
        self.board[pos] = value

    def __getitem__(self, pos):
        return self.board[pos]

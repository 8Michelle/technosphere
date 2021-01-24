import sys


def safe_input():
    try:
        return input()
    except EOFError:
        sys.exit(0)


class Interactor:
    def __init__(self, x):
        self.x = x
        self.coins = 0

    def check(self, values):
        results = [int(self.x < a) for a in values]
        self.coins += (len(results) + 10)
        return results

    def checkAnswer(self, a):
        """if a == self.x:
            print('correct')
            print(self.coins)"""
        #print(self.x, a, self.coins)
        #print((a == self.x) and (self.coins <= 100))
        return (a == self.x) and (self.coins <= 100)


def makeRequests(left, right, interactor):
    length = right - left
    #print(length)
    #print(left, right)
    if length == 0:
        return interactor.checkAnswer(left - 1)
    medians = [median for median in range(right, left - 1, -length // 10)]
    medians.reverse()
    #print(medians)
    responses = interactor.check(medians)
    #print(responses)
    if responses[-1] == 0:
        #left = medians[-1]
        #return makeRequests(left, right, interactor)
        return interactor.checkAnswer(medians[-1])
    else:
        right = medians[responses.index(1)]
        left = right - length // 10
        return makeRequests(left, right, interactor)


if __name__ == "__main__":
    for i in range(1, 100001):
        interactor = Interactor(x=i)
        if not makeRequests(1, 100000, interactor):
            print(i)
    #interactor = Interactor(x=25)
    #print(makeRequests(1, 100000, interactor))
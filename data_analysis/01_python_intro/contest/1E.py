import sys

def safe_input():
    try:
        return input()
    except EOFError:
        sys.exit(0)

def makeRequests(left=1, right=100000):
    length = right - left
    if length == 0:
        print("!", left - 1, flush=True)
        return
    medians = [median for median in range(right, left - 1, -length // 10)]
    medians.reverse()
    for median in medians:
        print("?", median, flush=True)
    print("+", flush=True)
    responses = [int(safe_input()) for _ in medians]
    if responses[-1] == 0:
        print("!", medians[-1], flush=True)
    else:
        right = medians[responses.index(1)]
        left = right - length // 10
        makeRequests(left, right)

if __name__ == "__main__":
    makeRequests()

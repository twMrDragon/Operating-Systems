import random
import sys

DISK_SIZE = 5000
NUM_REQUESTS = 1000

def generate_requests():
    return [random.randint(0, DISK_SIZE - 1) for _ in range(NUM_REQUESTS)]

def fcfs(requests, head):
    movement = 0
    for req in requests:
        movement += abs(head - req)
        head = req
    return movement

def scan(requests, head):
    movement = 0
    requests.sort()
    left = [r for r in requests if r < head]
    right = [r for r in requests if r >= head]

    # Service right, then left
    for r in right:
        movement += abs(head - r)
        head = r
    if left:
        # Move to the end then reverse
        movement += abs(head - (DISK_SIZE - 1))
        head = DISK_SIZE - 1
        movement += abs(head - left[-1])
        for r in reversed(left):
            movement += abs(head - r)
            head = r
    return movement

def cscan(requests, head):
    movement = 0
    requests.sort()
    left = [r for r in requests if r < head]
    right = [r for r in requests if r >= head]

    # Service right side
    for r in right:
        movement += abs(head - r)
        head = r
    if left:
        # Jump to start of disk
        movement += abs(head - (DISK_SIZE - 1))
        movement += DISK_SIZE - 1  # Move from end to start (0)
        head = 0
        for r in left:
            movement += abs(head - r)
            head = r
    return movement

def main():
    if len(sys.argv) != 2:
        print("Usage: python disk_scheduling.py <initial_head_position>")
        return

    try:
        head = int(sys.argv[1])
        if not (0 <= head < DISK_SIZE):
            raise ValueError()
    except ValueError:
        print(f"Initial head position must be an integer between 0 and {DISK_SIZE - 1}.")
        return

    requests = generate_requests()
    print(f"Initial head position: {head}")
    print(f"Generated {NUM_REQUESTS} random requests.")

    total_fcfs = fcfs(requests, head)
    total_scan = scan(requests, head)
    total_cscan = cscan(requests, head)

    print("\nTotal Head Movements:")
    print(f"FCFS:   {total_fcfs}")
    print(f"SCAN:   {total_scan}")
    print(f"C-SCAN: {total_cscan}")

if __name__ == "__main__":
    main()
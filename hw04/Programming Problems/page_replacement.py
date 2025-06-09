import random
import sys

def generate_reference_string(length=20, page_range=10):
    return [random.randint(0, page_range - 1) for _ in range(length)]

def fifo(ref_string, frame_count):
    frames = []
    page_faults = 0
    for page in ref_string:
        if page not in frames:
            if len(frames) >= frame_count:
                frames.pop(0)
            frames.append(page)
            page_faults += 1
    return page_faults

def lru(ref_string, frame_count):
    frames = []
    recent_use = {}
    page_faults = 0
    for i, page in enumerate(ref_string):
        if page not in frames:
            if len(frames) >= frame_count:
                # Find least recently used
                lru_page = min(frames, key=lambda x: recent_use[x])
                frames.remove(lru_page)
            frames.append(page)
            page_faults += 1
        recent_use[page] = i
    return page_faults

def optimal(ref_string, frame_count):
    frames = []
    page_faults = 0
    for i, page in enumerate(ref_string):
        if page not in frames:
            if len(frames) >= frame_count:
                # Replace the page with the farthest next use
                future_uses = {}
                for f in frames:
                    try:
                        future_uses[f] = ref_string[i+1:].index(f)
                    except ValueError:
                        future_uses[f] = float('inf')
                to_remove = max(future_uses, key=future_uses.get)
                frames.remove(to_remove)
            frames.append(page)
            page_faults += 1
    return page_faults

def main():
    if len(sys.argv) < 2:
        print("Usage: python page_replacement.py <number_of_frames>")
        return
    
    frame_count = int(sys.argv[1])
    reference_string = generate_reference_string()

    print(f"Reference String: {reference_string}")
    print(f"Number of Frames: {frame_count}\n")

    fifo_faults = fifo(reference_string, frame_count)
    lru_faults = lru(reference_string, frame_count)
    opt_faults = optimal(reference_string, frame_count)

    print(f"FIFO Page Faults: {fifo_faults}")
    print(f"LRU Page Faults:  {lru_faults}")
    print(f"OPT Page Faults:  {opt_faults}")

if __name__ == "__main__":
    main()

import tkinter as tk
from tkinter import ttk

class TuringMachine:
    def __init__(self, tape, low, high, target, gui):
        self.tape = tape
        self.low = low
        self.high = high
        self.target = target
        self.head = 0
        self.steps = 0
        self.grids = 0
        self.gui = gui
        self.mid = (self.low + self.high) // 2
        self.state = 'initLow'
        self.low_assigned = False
        self.high_assigned = False
        self.mid_assigned = False
        self.update_gui()

    def move_down(self):
        self.head += 1
        self.update_gui("Down")

    def move_up(self):
        self.head -= 1
        self.update_gui("Up")

    def move_to_position(self, position):
        if self.head < position:
            self.move_down()
            return False
        elif self.head > position:
            self.move_up()
            return False
        return True

    def step(self):
        if self.state == 'initLow':
            self.steps += 1
            self.gui.update_status(f"State: {self.state}")
            if not self.low_assigned:
                self.low_assigned = True
                self.grids += 1
            self.state = 'writeLow'
        elif self.state == 'writeLow':
            self.steps += 1
            self.gui.update_status(f"State: {self.state}")
            self.state = 'initHigh'
        elif self.state == 'initHigh':
            self.steps += 1
            self.gui.update_status(f"State: {self.state}")
            if not self.high_assigned:
                self.high_assigned = True
                self.grids += 1
            self.state = 'writeHigh'
        elif self.state == 'writeHigh':
            self.steps += 1
            self.gui.update_status(f"State: {self.state}")
            self.state = 'readK'
        elif self.state == 'readK':
            self.steps += 1
            self.gui.update_status(f"State: {self.state}")
            self.state = 'compareLow'
        elif self.state == 'compareLow':
            self.steps += 1
            self.gui.update_status(f"State: {self.state}")
            if self.low <= self.high:
                self.state = 'calMid'
            else:
                self.state = 'stop'
        elif self.state == 'calMid':
            self.steps += 1
            self.gui.update_status(f"State: {self.state}")
            if not self.mid_assigned:
                self.mid_assigned = True
                self.grids += 1
            self.mid = (self.low + self.high) // 2
            self.state = 'readMid'
        elif self.state == 'readMid':
            self.gui.update_status(f"State: {self.state}")
            if self.move_to_position(self.mid):
                self.steps += 1
                self.state = 'compareMid'
        elif self.state == 'compareMid':
            self.steps += 1
            self.gui.update_status(f"State: {self.state}")
            if self.tape[self.mid] == self.target:
                self.state = 'success'
            elif self.tape[self.mid] < self.target:
                self.low = self.mid + 1
                self.state = 'updateLow'
            else:
                self.high = self.mid - 1
                self.state = 'updateHigh'
        elif self.state == 'updateLow':
            self.steps += 1
            self.gui.update_status(f"State: {self.state}")
            self.state = 'compareLow'
        elif self.state == 'updateHigh':
            self.steps += 1
            self.gui.update_status(f"State: {self.state}")
            self.state = 'compareLow'
        elif self.state == 'success':
            self.steps += 1
            self.gui.update_status(f"State: {self.state}, The index of x is {self.mid}")
            self.gui.update_result("success")
            self.state = 'stop'
        elif self.state == 'stop':
            self.gui.update_status("State: stop, The index of x is -1")
            self.gui.next_button.config(state=tk.DISABLED)
            self.gui.update_result("fail")
        self.update_gui()

    def update_gui(self, direction="None"):
        self.gui.update_tape(self.tape, self.head)
        self.gui.update_info(direction, self.steps, self.grids)
        self.gui.update_status(f"Current State: {self.state}")
        self.gui.update_bounds(self.low, self.high, self.mid)

class TuringMachineGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Turing Machine Simulation")
        self.root.geometry("500x700")
        self.root.resizable(False, False)

        style = ttk.Style()
        style.configure("TButton", font=("Helvetica", 12))
        style.configure("TLabel", font=("Helvetica", 12))

        self.tape_frame = tk.Frame(root)
        self.tape_frame.pack(pady=10)

        self.canvas = tk.Canvas(self.tape_frame, width=400, height=400)
        self.scrollbar = ttk.Scrollbar(self.tape_frame, orient="vertical", command=self.canvas.yview)
        self.scrollable_frame = ttk.Frame(self.canvas)

        self.scrollable_frame.bind(
            "<Configure>",
            lambda e: self.canvas.configure(
                scrollregion=self.canvas.bbox("all")
            )
        )

        self.canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")
        self.canvas.configure(yscrollcommand=self.scrollbar.set)

        self.canvas.pack(side="left", fill="both", expand=True)
        self.scrollbar.pack(side="right", fill="y")

        self.tape_labels = []
        for _ in range(40):  # 初始占位，最多显示40个数
            label = ttk.Label(self.scrollable_frame, text="", font=("Courier", 16), anchor="w")
            label.pack(anchor="w", pady=2)
            self.tape_labels.append(label)

        self.info_label = ttk.Label(root, text="Direction: None, Steps: 0, Grids: 0")
        self.info_label.pack(pady=10)

        self.status_label = ttk.Label(root, text="State: initLow")
        self.status_label.pack(pady=10)

        self.bounds_label = ttk.Label(root, text="low: 0, high: 0, mid: 0")
        self.bounds_label.pack(pady=10)

        self.input_label = ttk.Label(root, text="请输入数据（每个数字之间用空格隔开）：")
        self.input_label.pack(pady=5)

        self.input_entry = ttk.Entry(root, width=50)
        self.input_entry.pack(pady=5)

        self.start_button = ttk.Button(root, text="Start", command=self.start_simulation)
        self.start_button.pack(pady=5)

        self.next_button = ttk.Button(root, text="Next Step", command=self.next_step, state=tk.DISABLED)
        self.next_button.pack(pady=5)

        self.result_label = ttk.Label(root, text="")
        self.result_label.pack(pady=20)  # 调整布局，使其单独显示在最下面

    def update_tape(self, tape, head):
        for i, value in enumerate(tape):
            if i < len(self.tape_labels):
                if i == head:
                    self.tape_labels[i].config(text=f"{value} <--", background="yellow")
                else:
                    self.tape_labels[i].config(text=str(value), background="white")

    def update_info(self, direction, steps, grids):
        self.info_label.config(text=f"Direction: {direction}, Steps: {steps}, Grids: {grids}")

    def update_status(self, status):
        self.status_label.config(text=status)

    def update_bounds(self, low, high, mid):
        self.bounds_label.config(text=f"low: {low}, high: {high}, mid: {mid}")

    def update_result(self, result):
        self.result_label.config(text=result)

    def start_simulation(self):
        input_data = self.input_entry.get().strip().split()
        input_data = list(map(int, input_data))
        low = input_data[0]
        high = input_data[1]
        target = input_data[2]
        tape = input_data[3:]

        self.tm = TuringMachine(tape, low, high, target, self)
        self.next_button.config(state=tk.NORMAL)

    def next_step(self):
        self.tm.step()
        if self.tm.state == 'stop':
            self.next_button.config(state=tk.DISABLED)
            if self.tm.tape[self.tm.mid] == self.tm.target:
                self.update_result("success")
            else:
                self.update_result("fail")


# 创建主窗口
root = tk.Tk()
gui = TuringMachineGUI(root)

# 运行主循环
root.mainloop()

import tkinter as tk
from tkinter import ttk
import time

class KnapsackDP:
    def __init__(self, W, n, items, gui):
        self.W = W
        self.n = n
        self.weights = [items[2 * i] for i in range(n)]
        self.values = [items[2 * i + 1] for i in range(n)]
        self.input_tape = items
        self.input_tape_head = 0
        self.gui = gui
        self.dp = [[0] * (W + 1) for _ in range(n)]
        self.work_tape = [0] * (n * (W + 1))
        self.steps = 0
        self.grids = 0
        self.i = 0
        self.w = 0
        self.head = 0
        self.phase = 'initial'
        self.reading_input = False
        self.sub_phase = 0
        self.highlighted_cells = set()
        self.highlighted_work_cells = set()
        self.states = ['readValue', 'readWeight', 'readNum', 'readCapacity', 'writeM', 'readM', 'calM', 'cmp', 'readW', 'writeAns', 'cmpAnsW', 'ansM', 'writeAns', 'success']
        self.current_state = self.states[0]
        self.direction = ""
        self.steps_moved = 0
        self.update_gui()

    def change_state(self, state):
        self.current_state = state
        self.update_gui()

    def move_head_step_by_step(self, new_position):
        step = 1 if self.head < new_position else -1
        self.direction = "Right" if step == 1 else "Left"
        self.steps_moved = abs(self.head - new_position)
        while self.head != new_position:
            self.head += step
            self.steps += 1
            self.update_gui()
            time.sleep(0.01)

    def step(self):
        if self.reading_input:
            # 模拟读取输入纸带
            self.change_state('readValue' if self.input_tape_head % 2 == 0 else 'readWeight')
            if self.input_tape_head < len(self.input_tape):
                self.input_tape_head += 1
                self.steps += 1  # 每次读取输入纸带计入steps
                self.update_gui()
                time.sleep(0.5)
                if self.input_tape_head % 2 == 0:
                    self.reading_input = False
        elif self.phase == 'initial':
            self.change_state('readNum')
            self.reading_input = True
            self.phase = 'dp_fill'
        elif self.phase == 'dp_fill':
            self.change_state('writeM')
            if self.i < self.n:
                if self.w <= self.W:
                    if self.sub_phase == 0:
                        self.move_head_step_by_step(self.i * (self.W + 1) + self.w)
                        self.update_gui()
                        self.sub_phase = 1
                    elif self.sub_phase == 1:
                        self.change_state('readM')
                        if self.i > 0 and self.w > 0:
                            self.highlight_path(self.i - 1, self.w - 1, self.i, self.w)
                            self.steps += abs(self.w - (self.w - 1))  # 比较计入steps
                        self.sub_phase = 2
                    elif self.sub_phase == 2:
                        self.change_state('calM')
                        if self.i > 0 and self.w > 0:
                            self.clear_highlight(self.i - 1, self.w - 1, self.i, self.w)
                        if self.i > 0:
                            self.highlight_path(self.i - 1, self.w, self.i, self.w)
                            self.steps += abs(self.w - (self.w - 1))  # 比较计入steps
                        self.sub_phase = 3
                    elif self.sub_phase == 3:
                        self.change_state('cmp')
                        if self.i > 0:
                            self.clear_highlight(self.i - 1, self.w, self.i, self.w)
                        self.highlight_path(self.i, self.w - 1, self.i, self.w)
                        self.steps += abs(self.w - (self.w - 1))  # 比较计入steps
                        self.sub_phase = 4
                    elif self.sub_phase == 4:
                        self.clear_highlight(self.i, self.w - 1, self.i, self.w)
                        self.change_state('readW')
                        if self.w == 0:
                            self.dp[self.i][self.w] = 0
                        elif self.weights[self.i] <= self.w:
                            self.dp[self.i][self.w] = max(
                                self.values[self.i] + self.dp[self.i - 1][self.w - self.weights[self.i]],
                                self.dp[self.i - 1][self.w]
                            )
                        else:
                            self.dp[self.i][self.w] = self.dp[self.i - 1][self.w]

                        # 更新工作带
                        self.work_tape[self.i * (self.W + 1) + self.w] = self.dp[self.i][self.w]
                        self.grids += 1
                        self.update_gui()
                        self.sub_phase = 0
                        self.w += 1
                        self.direction = "Right"
                        self.steps_moved = 1
                else:
                    self.w = 0
                    self.i += 1
                    if self.i < self.n:
                        self.reading_input = True
            else:
                self.change_state('success')
                self.phase = 'done'
                self.gui.next_button.config(state=tk.DISABLED)
                self.gui.auto_run_button.config(state=tk.DISABLED)
                self.gui.pause_button.config(state=tk.DISABLED)
                self.gui.speed_up_button.config(state=tk.DISABLED)
                self.gui.speed_down_button.config(state=tk.DISABLED)

    def highlight_path(self, start_i, start_j, end_i, end_j):
        self.highlighted_cells.clear()
        self.highlighted_work_cells.clear()
        if start_i == end_i:
            # 同一行时只处理它们之间的方框变绿
            self.direction = "Left"
            self.steps_moved = abs(end_j - start_j)
            for c in range(start_j, end_j + 1):
                if 0 <= start_i < len(self.dp) and 0 <= c < len(self.dp[0]):
                    self.highlighted_cells.add((start_i, c))
                    self.highlighted_work_cells.add(start_i * (self.W + 1) + c)
        else:
            # Highlight the top-left to current cell path
            self.direction = "Left"
            self.steps_moved = 0
            if start_i >= 0:
                for c in range(start_j, self.W + 1):
                    if 0 <= start_i < len(self.dp) and 0 <= c < len(self.dp[0]):
                        self.highlighted_cells.add((start_i, c))
                        self.highlighted_work_cells.add(start_i * (self.W + 1) + c)
                        self.steps_moved += 1
            for c in range(0, end_j + 1):
                if 0 <= end_i < len(self.dp) and 0 <= c < len(self.dp[0]):
                    self.highlighted_cells.add((end_i, c))
                    self.highlighted_work_cells.add(end_i * (self.W + 1) + c)
                    self.steps_moved += 1
        self.update_gui()

    def clear_highlight(self, start_i, start_j, end_i, end_j):
        self.highlighted_cells.clear()
        self.highlighted_work_cells.clear()
        self.update_gui()

    def update_gui(self):
        self.gui.update_table(self.dp, self.i, self.w, self.highlighted_cells)
        self.gui.update_work_tape(self.work_tape, self.i * (self.W + 1) + self.w, self.highlighted_work_cells)
        self.gui.update_info(self.steps, self.grids, self.current_state, self.direction, self.steps_moved)
        self.gui.update_input_tape(self.input_tape, self.input_tape_head)

class KnapsackDPGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Knapsack DP Simulation")
        self.root.geometry("1200x700")
        self.root.resizable(True, True)
        self.root.configure(bg="#f0f0f0")

        self.table_frame = ttk.Frame(root, padding=10)
        self.table_frame.pack(pady=10)

        self.work_tape_canvas = tk.Canvas(root, height=50, bg="#e0e0e0")
        self.work_tape_scrollbar = ttk.Scrollbar(root, orient="horizontal", command=self.work_tape_canvas.xview)
        self.work_tape_frame = ttk.Frame(self.work_tape_canvas, padding=10, style="Tape.TFrame")

        self.work_tape_frame.bind(
            "<Configure>",
            lambda e: self.work_tape_canvas.configure(
                scrollregion=self.work_tape_canvas.bbox("all")
            )
        )

        self.work_tape_canvas.create_window((0, 0), window=self.work_tape_frame, anchor="nw")
        self.work_tape_canvas.configure(xscrollcommand=self.work_tape_scrollbar.set)

        self.work_tape_canvas.pack(fill="both", expand=True)
        self.work_tape_scrollbar.pack(fill="x")

        self.input_tape_frame = ttk.Frame(root, padding=10)
        self.input_tape_frame.pack(pady=10)

        self.info_frame = ttk.Frame(root, padding=10)
        self.info_frame.pack(pady=10)

        self.state_label = ttk.Label(root, text="", background="#f0f0f0", font=("Helvetica", 14, "bold"))
        self.state_label.pack(pady=5)

        self.direction_label = ttk.Label(root, text="", background="#f0f0f0", font=("Helvetica", 12))
        self.direction_label.pack(pady=5)

        self.input_label = ttk.Label(root, text="请输入数据（每个数字之间用空格隔开）：", background="#f0f0f0", font=("Helvetica", 12))
        self.input_label.pack(pady=5)

        self.input_entry = ttk.Entry(root, width=50, font=("Helvetica", 12))
        self.input_entry.pack(pady=5)

        self.button_frame = ttk.Frame(root, padding=10)
        self.button_frame.pack(pady=10)

        self.start_button = ttk.Button(self.button_frame, text="Start", command=self.start_simulation, style="TButton")
        self.start_button.grid(row=0, column=0, padx=5)

        self.next_button = ttk.Button(self.button_frame, text="Next Step", command=self.next_step, state=tk.DISABLED, style="TButton")
        self.next_button.grid(row=0, column=1, padx=5)

        self.run_button_frame = ttk.Frame(root, padding=10)
        self.run_button_frame.pack(pady=10)

        self.auto_run_button = ttk.Button(self.run_button_frame, text="Auto Run", command=self.auto_run, state=tk.DISABLED, style="TButton")
        self.auto_run_button.grid(row=0, column=0, padx=5)

        self.pause_button = ttk.Button(self.run_button_frame, text="Pause", command=self.toggle_pause, state=tk.DISABLED, style="TButton")
        self.pause_button.grid(row=0, column=1, padx=5)

        self.speed_up_button = ttk.Button(self.run_button_frame, text="Speed Up", command=self.speed_up, state=tk.DISABLED, style="TButton")
        self.speed_up_button.grid(row=0, column=2, padx=5)

        self.speed_down_button = ttk.Button(self.run_button_frame, text="Speed Down", command=self.speed_down, state=tk.DISABLED, style="TButton")
        self.speed_down_button.grid(row=0, column=3, padx=5)

        self.style = ttk.Style()
        self.style.configure("TButton", font=("Helvetica", 12), padding=5)
        self.style.configure("Tape.TFrame", background="#e0e0e0")

        self.paused = False
        self.speed_levels = [0.5, 0.2, 0.1, 0.05, 0.01]
        self.current_speed = 2  # 中间档速度

    def update_table(self, dp, i, w, highlighted_cells):
        for widget in self.table_frame.winfo_children():
            widget.destroy()
        for r, row in enumerate(dp):
            for c, val in enumerate(row):
                color = "white"
                if (r, c) in highlighted_cells:
                    color = "#ccffcc"  # 透明绿色
                if r == i and c == w:
                    color = "yellow"
                label = ttk.Label(self.table_frame, text=str(val), font=("Courier", 12), borderwidth=1, relief="solid", width=5, background=color)
                label.grid(row=r, column=c, padx=1, pady=1)

    def update_work_tape(self, work_tape, head, highlighted_work_cells):
        for widget in self.work_tape_frame.winfo_children():
            widget.destroy()
        for index, value in enumerate(work_tape):
            color = "white"
            if index in highlighted_work_cells:
                color = "#ccffcc"
            if index == head:
                color = "yellow"
            label = ttk.Label(self.work_tape_frame, text=str(value), font=("Courier", 12), borderwidth=1, relief="solid", width=5, background=color)
            label.grid(row=0, column=index, padx=1, pady=1)

    def update_info(self, steps, grids, current_state, direction, steps_moved):
        for widget in self.info_frame.winfo_children():
            widget.destroy()
        info_label = ttk.Label(self.info_frame, text=f"Steps: {steps}, Grids: {grids}", background="#f0f0f0", font=("Helvetica", 12))
        info_label.pack(side=tk.LEFT, padx=5)
        self.state_label.config(text=f"State: {current_state}")
        self.direction_label.config(text=f"Direction: {direction}, Steps Moved: {steps_moved}")

    def update_input_tape(self, input_tape, input_head):
        for widget in self.input_tape_frame.winfo_children():
            widget.destroy()
        for index, value in enumerate(input_tape):
            label = ttk.Label(self.input_tape_frame, text=str(value), font=("Courier", 12), borderwidth=1, relief="solid", width=5)
            label.grid(row=0, column=index, padx=1, pady=1)
            if index == input_head:
                label.config(background="yellow")

    def start_simulation(self):
        input_data = self.input_entry.get().strip().split()
        input_data = list(map(int, input_data))
        W = input_data[0]
        n = input_data[1]
        items = input_data[2:]
        
        self.dp_simulation = KnapsackDP(W, n, items, self)
        self.next_button.config(state=tk.NORMAL)
        self.auto_run_button.config(state=tk.NORMAL)
        self.pause_button.config(state=tk.DISABLED)
        self.speed_up_button.config(state=tk.DISABLED)
        self.speed_down_button.config(state=tk.DISABLED)

    def next_step(self):
        self.dp_simulation.step()
        if self.dp_simulation.phase == 'done':
            self.next_button.config(state=tk.DISABLED)
            self.auto_run_button.config(state=tk.DISABLED)
            self.pause_button.config(state=tk.DISABLED)
            self.speed_up_button.config(state=tk.DISABLED)
            self.speed_down_button.config(state=tk.DISABLED)

    def auto_run(self):
        self.paused = False
        self.pause_button.config(state=tk.NORMAL, text="Pause")
        self.speed_up_button.config(state=tk.NORMAL)
        self.speed_down_button.config(state=tk.NORMAL)
        self._auto_run()

    def _auto_run(self):
        while self.dp_simulation.phase != 'done' and not self.paused:
            self.dp_simulation.step()
            self.root.update()
            time.sleep(self.speed_levels[self.current_speed])
        if self.dp_simulation.phase == 'done':
            self.next_button.config(state=tk.DISABLED)
            self.auto_run_button.config(state=tk.DISABLED)
            self.pause_button.config(state=tk.DISABLED)
            self.speed_up_button.config(state=tk.DISABLED)
            self.speed_down_button.config(state=tk.DISABLED)

    def toggle_pause(self):
        if self.paused:
            self.paused = False
            self.pause_button.config(text="Pause")
            self._auto_run()
        else:
            self.paused = True
            self.pause_button.config(text="Continue")

    def speed_up(self):
        if self.current_speed < len(self.speed_levels) - 1:
            self.current_speed += 1

    def speed_down(self):
        if self.current_speed > 0:
            self.current_speed -= 1

# 创建主窗口
root = tk.Tk()
gui = KnapsackDPGUI(root)

# 运行主循环
root.mainloop()
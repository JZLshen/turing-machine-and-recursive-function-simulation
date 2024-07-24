import tkinter as tk
from tkinter import ttk
import heapq

class Node:
    def __init__(self, index, cw, cp, up):
        self.index = index
        self.cw = cw  # current weight
        self.cp = cp  # current profit
        self.up = up  # upper bound (best possible profit)
        self.parent = None
        self.include = None  # Whether this node includes the item
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.up > other.up  # max-heap based on upper bound

class KnapsackBranchBound:
    def __init__(self, W, n, items, gui):
        self.W = W
        self.n = n
        self.items = [(items[2 * i], items[2 * i + 1]) for i in range(n)]
        self.sorted_indices = sorted(range(n), key=lambda i: items[2 * i + 1] / items[2 * i], reverse=True)
        self.gui = gui
        self.root = Node(index=-1, cw=0, cp=0, up=self.bound(0, 0, 0, W, self.sorted_indices))
        self.priority_queue = []
        heapq.heappush(self.priority_queue, (-self.root.up, self.root))  # use negative up for max-heap
        self.bestp = 0
        self.best_node = None
        self.nodes = [self.root]
        self.current_node = None
        self.phase = 'read_input'
        self.input_tape_head = 0
        self.work_tape_head = 0
        self.work_tape = [None] * (n * 2)  # 预设一定数量的格子
        self.output_tape = [0] * n
        self.steps = 0  # 读头走过的长度
        self.grids = 0  # 工作纸带写入的方格数
        self.auto_running = False  # 新增自动运行标志
        self.paused = False  # 自动运行暂停标志
        self.current_level = 0  # 当前处理的层数
        self.previous_level = 0  # 上次处理的树的层数
        self.previous_position = 0
        self.update_gui()

    def bound(self, index, cw, cp, W, sorted_indices):
        if cw > W:
            return 0
        profit_bound = cp
        total_weight = cw
        n = len(sorted_indices)

        for i in range(index, n):
            item_index = sorted_indices[i]
            item_weight, item_value = self.items[item_index]
            if total_weight + item_weight <= W:
                total_weight += item_weight
                profit_bound += item_value
            else:
                profit_bound += (W - total_weight) * (item_value / item_weight)
                break

        return profit_bound

    def read_input(self):
        if self.input_tape_head < len(self.items) * 2:
            item_index = self.input_tape_head // 2
            is_weight = self.input_tape_head % 2 == 0
            self.gui.update_input_tape(self.items, self.input_tape_head, self.current_level)
            self.steps += abs(self.input_tape_head - self.previous_position)
            self.previous_position = self.input_tape_head
            if not is_weight:
                weight, value = self.items[item_index]
                ratio = value / weight
                self.work_tape[self.work_tape_head] = ratio
                self.work_tape[self.work_tape_head + 1] = item_index
                self.work_tape_head += 2
                self.grids += 1
                self.gui.update_work_tape(self.work_tape, self.work_tape_head - 2, self.current_level)
            self.input_tape_head += 1
        else:
            self.sort_work_tape()
            self.phase = 'process_queue'
        self.update_gui()

    def sort_work_tape(self):
        ratios = [(self.work_tape[i], self.work_tape[i + 1]) for i in range(0, self.work_tape_head, 2)]
        sorted_ratios = sorted(ratios, key=lambda x: x[0], reverse=True)
        self.work_tape = [None] * len(self.work_tape)
        for i, (ratio, index) in enumerate(sorted_ratios):
            self.work_tape[2 * i] = ratio
            self.work_tape[2 * i + 1] = index
        self.sorted_indices = [int(index) for _, index in sorted_ratios]
        self.gui.update_work_tape(self.work_tape, current_level=self.current_level)

    def update_gui(self):
        self.gui.update_tree(self.nodes, self.best_node, self.priority_queue)
        self.gui.update_info(self.phase)
        self.gui.update_output_tape(self.output_tape)
        self.gui.update_binary_tree(self.root)
        self.gui.update_status(self.steps, self.grids, self.bestp)
        self.gui.root.update_idletasks()

    def step(self):
        if self.phase == 'read_input':
            self.read_input()
        elif self.phase == 'sort_work_tape':
            self.sort_work_tape()
        elif self.phase == 'process_queue':
            if not self.priority_queue:
                self.phase = 'done'
            else:
                self.current_node = heapq.heappop(self.priority_queue)[1]
                self.phase = 'expand_node'
        elif self.phase == 'expand_node':
            self.expand_node()

        self.update_gui()
        self.print_priority_queue()

    def expand_node(self):
        index = self.current_node.index + 1
        self.current_level = index + 1  # 更新当前处理的层数

        if index < self.n:
            item_index = self.sorted_indices[index]
            item_weight, item_value = self.items[item_index]

            left_cw = self.current_node.cw + item_weight
            left_cp = self.current_node.cp + item_value
            left_up = self.bound(index + 1, left_cw, left_cp, self.W, self.sorted_indices)
            left_child = Node(index=index, cw=left_cw, cp=left_cp, up=left_up)
            left_child.parent = self.current_node
            left_child.include = True
            self.current_node.left = left_child

            if left_cw <= self.W and left_cp >= self.bestp:
                self.bestp = left_cp
                self.best_node = left_child

            if left_cw <= self.W:
                heapq.heappush(self.priority_queue, (-left_child.up, left_child))
                self.nodes.append(left_child)

            right_cw = self.current_node.cw
            right_cp = self.current_node.cp
            right_up = self.bound(index + 1, right_cw, right_cp, self.W, self.sorted_indices)
            right_child = Node(index=index, cw=right_cw, cp=right_cp, up=right_up)
            right_child.parent = self.current_node
            right_child.include = False
            self.current_node.right = right_child

            if right_up >= self.bestp:
                heapq.heappush(self.priority_queue, (-right_up, right_child))
                self.nodes.append(right_child)

        # Update steps and previous_level
        self.steps += abs(self.current_level - self.previous_level)
        self.previous_level = self.current_level

        # Update GUI for current level
        self.gui.update_input_tape(self.items, self.input_tape_head, self.current_level)
        self.gui.update_work_tape(self.work_tape, self.work_tape_head - 2, self.current_level)

        # Check if we have reached a leaf node and cw is within the weight limit
        if self.current_node.index == self.n - 1 and self.current_node.cw <= self.W:
            self.phase = 'done'
            self.update_output_tape()
            self.gui.next_button.config(state=tk.DISABLED)
            self.auto_running = False
        else:
            self.phase = 'process_queue'

    def update_output_tape(self):
        selected_items = [0] * self.n
        node = self.best_node
        while node and node.index >= 0:
            selected_items[self.sorted_indices[node.index]] = 1 if node.include else 0
            node = node.parent
        self.output_tape = selected_items
        print("Selected items indices:", [i for i, x in enumerate(self.output_tape) if x == 1])

    def print_priority_queue(self):
        print("Priority Queue:")
        for up, node in self.priority_queue:
            print(f"Index: {node.index}, CW: {node.cw}, CP: {node.cp}, UP: {-up}")

class KnapsackBranchBoundGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Knapsack Branch and Bound Simulation")
        self.root.geometry("1200x700")
        self.root.resizable(True, True)
        self.root.configure(bg="#f0f0f0")

        self.paned_window = ttk.PanedWindow(self.root, orient=tk.HORIZONTAL)
        self.paned_window.pack(fill=tk.BOTH, expand=True)

        self.left_frame = ttk.Frame(self.paned_window, padding=10)
        self.right_frame = ttk.Frame(self.paned_window, padding=10)
        
        self.paned_window.add(self.left_frame, weight=3)
        self.paned_window.add(self.right_frame, weight=1)

        self.canvas_input = tk.Canvas(self.left_frame, height=50)
        self.canvas_work = tk.Canvas(self.left_frame, height=50)
        self.canvas_tree = tk.Canvas(self.left_frame)
        self.canvas_output = tk.Canvas(self.left_frame, height=50)

        self.input_tape_frame = ttk.Frame(self.canvas_input)
        self.work_tape_frame = ttk.Frame(self.canvas_work)
        self.tree_frame = ttk.Frame(self.canvas_tree)
        self.output_tape_frame = ttk.Frame(self.canvas_output)

        self.input_tape_scroll = ttk.Scrollbar(self.left_frame, orient=tk.HORIZONTAL, command=self.canvas_input.xview, style="TScrollbar")
        self.work_tape_scroll = ttk.Scrollbar(self.left_frame, orient=tk.HORIZONTAL, command=self.canvas_work.xview, style="TScrollbar")
        self.tree_scroll = ttk.Scrollbar(self.left_frame, orient=tk.VERTICAL, command=self.canvas_tree.yview, style="TScrollbar")
        self.output_tape_scroll = ttk.Scrollbar(self.left_frame, orient=tk.HORIZONTAL, command=self.canvas_output.xview, style="TScrollbar")

        self.canvas_input.configure(xscrollcommand=self.input_tape_scroll.set)
        self.canvas_work.configure(xscrollcommand=self.work_tape_scroll.set)
        self.canvas_tree.configure(yscrollcommand=self.tree_scroll.set)
        self.canvas_output.configure(xscrollcommand=self.output_tape_scroll.set)

        self.canvas_input.create_window((0, 0), window=self.input_tape_frame, anchor="nw")
        self.canvas_work.create_window((0, 0), window=self.work_tape_frame, anchor="nw")
        self.canvas_tree.create_window((0, 0), window=self.tree_frame, anchor="nw")
        self.canvas_output.create_window((0, 0), window=self.output_tape_frame, anchor="nw")

        self.input_tape_frame.bind("<Configure>", lambda e: self.canvas_input.configure(scrollregion=self.canvas_input.bbox("all")))
        self.work_tape_frame.bind("<Configure>", lambda e: self.canvas_work.configure(scrollregion=self.canvas_work.bbox("all")))
        self.tree_frame.bind("<Configure>", lambda e: self.canvas_tree.configure(scrollregion=self.canvas_tree.bbox("all")))
        self.output_tape_frame.bind("<Configure>", lambda e: self.canvas_output.configure(scrollregion=self.canvas_output.bbox("all")))

        self.canvas_input.pack(fill=tk.X, padx=10, pady=5)
        self.input_tape_scroll.pack(fill=tk.X, padx=10, pady=5)
        self.canvas_work.pack(fill=tk.X, padx=10, pady=5)
        self.work_tape_scroll.pack(fill=tk.X, padx=10, pady=5)
        self.canvas_tree.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        self.tree_scroll.pack(side=tk.RIGHT, fill=tk.Y)
        self.canvas_output.pack(fill=tk.X, padx=10, pady=5)
        self.output_tape_scroll.pack(fill=tk.X, padx=10, pady=5)

        self.info_frame = ttk.Frame(self.left_frame, padding=10)
        self.info_frame.pack(pady=10)

        self.state_label = ttk.Label(self.left_frame, text="", background="#f0f0f0", font=("Helvetica", 14, "bold"))
        self.state_label.pack(pady=5)

        self.input_label = ttk.Label(self.left_frame, text="请输入数据（每个数字之间用空格隔开）：", background="#f0f0f0", font=("Helvetica", 12))
        self.input_label.pack(pady=5)

        self.input_entry = ttk.Entry(self.left_frame, width=50, font=("Helvetica", 12))
        self.input_entry.pack(pady=5)

        self.button_frame = ttk.Frame(self.left_frame, padding=10)
        self.button_frame.pack(pady=10)

        self.start_button = ttk.Button(self.button_frame, text="Start", command=self.start_simulation, style="TButton")
        self.start_button.grid(row=0, column=0, padx=5)

        self.next_button = ttk.Button(self.button_frame, text="Next Step", command=self.next_step, state=tk.DISABLED, style="TButton")
        self.next_button.grid(row=0, column=1, padx=5)

        self.auto_run_button = ttk.Button(self.button_frame, text="Auto Run", command=self.auto_run, state=tk.DISABLED, style="TButton")
        self.auto_run_button.grid(row=0, column=2, padx=5)

        self.pause_button = ttk.Button(self.button_frame, text="Pause", command=self.pause_auto_run, state=tk.DISABLED, style="TButton")
        self.pause_button.grid(row=0, column=3, padx=5)

        self.speed_scale = ttk.Scale(self.button_frame, from_=1, to=5, orient=tk.HORIZONTAL, command=self.change_speed)
        self.speed_scale.set(3)  # 设置默认速度为中间值
        self.speed_scale.grid(row=0, column=4, padx=5)

        self.tree_canvas = tk.Canvas(self.right_frame, bg="white")
        self.tree_canvas.pack(fill=tk.BOTH, expand=True)
        self.style = ttk.Style()
        self.style.configure("TButton", font=("Helvetica", 12), padding=5)
        self.style.configure("TScrollbar", troughcolor="lightgray", background="darkgray", bordercolor="gray", arrowcolor="black")
        self.style.map("TScrollbar",
            background=[('active', 'black')],
            arrowcolor=[('active', 'white')],
            bordercolor=[('active', 'black')],
            troughcolor=[('active', 'lightgray')])

        self.status_frame = ttk.Frame(self.left_frame, padding=10)
        self.status_frame.pack(pady=10)
        self.status_label = ttk.Label(self.status_frame, text="", font=("Helvetica", 12))
        self.status_label.pack(pady=5)

        self.speed_delay = 500  # 默认速度为中间值

    def update_input_tape(self, items, tape_head, current_level=None):
        for widget in self.input_tape_frame.winfo_children():
            widget.destroy()
        for idx, (weight, value) in enumerate(items):
            text_w = f"{weight}"
            text_v = f"{value}"
            label_w = ttk.Label(self.input_tape_frame, text=text_w, font=("Courier", 12), borderwidth=1, relief="solid", width=5)
            label_v = ttk.Label(self.input_tape_frame, text=text_v, font=("Courier", 12), borderwidth=1, relief="solid", width=5)
            label_w.grid(row=0, column=2*idx, padx=1, pady=1)
            label_v.grid(row=0, column=2*idx+1, padx=1, pady=1)
            if tape_head == 2*idx:
                label_w.config(background="yellow")
            elif tape_head == 2*idx+1:
                label_v.config(background="yellow")
            if current_level is not None and current_level - 1 == idx:
                label_w.config(background="lightgreen")
                label_v.config(background="lightgreen")

    def update_work_tape(self, work_tape, tape_head=None, current_level=None):
        for widget in self.work_tape_frame.winfo_children():
            widget.destroy()
        for index, value in enumerate(work_tape):
            if value is not None:
                text = f"{value:.2f}" if index % 2 == 0 else f"{int(value)}"
                label = ttk.Label(self.work_tape_frame, text=text, font=("Courier", 12), borderwidth=1, relief="solid", width=10)
                label.grid(row=0, column=index, padx=1, pady=1)
                if tape_head is not None and index == tape_head:
                    label.config(background="yellow")
                if current_level is not None and current_level - 1 == index // 2:
                    label.config(background="lightgreen")



    def update_tree(self, nodes, best_node, priority_queue):
        for widget in self.tree_frame.winfo_children():
            widget.destroy()
        pq_indices = {node.index for _, node in priority_queue}
        level = {node.index: node for _, node in priority_queue}
        for node in nodes:
            label_text = f"Index: {node.index}, CW: {node.cw}, CP: {node.cp}, UP: {node.up:.2f}"
            label = ttk.Label(self.tree_frame, text=label_text, font=("Courier", 12), borderwidth=1, relief="solid", width=50)
            label.pack(pady=2)
            if node == best_node:
                label.config(background="yellow")
            elif node.index in pq_indices and level[node.index] == node:
                label.config(background="lightgreen")

    def update_output_tape(self, output_tape):
        for widget in self.output_tape_frame.winfo_children():
            widget.destroy()
        for index, value in enumerate(output_tape):
            label = ttk.Label(self.output_tape_frame, text=str(value), font=("Courier", 12), borderwidth=1, relief="solid", width=5)
            label.grid(row=0, column=index, padx=1, pady=1)

    def update_info(self, phase):
        self.state_label.config(text=f"Phase: {phase}")

    def update_status(self, steps, grids, bestp):
        self.status_label.config(text=f"Steps: {steps+grids*2}, Grids: {grids*2}, Best Profit: {bestp}")

    def start_simulation(self):
        input_data = self.input_entry.get().strip().split()
        input_data = list(map(int, input_data))
        W = input_data[0]
        n = input_data[1]
        items = input_data[2:]
        
        self.knapsack_simulation = KnapsackBranchBound(W, n, items, self)
        self.next_button.config(state=tk.NORMAL)
        self.auto_run_button.config(state=tk.NORMAL)

    def next_step(self):
        self.knapsack_simulation.step()
        if self.knapsack_simulation.phase == 'done':
            self.next_button.config(state=tk.DISABLED)
            self.auto_run_button.config(state=tk.DISABLED)
            self.pause_button.config(state=tk.DISABLED)

    def auto_run(self):
        self.knapsack_simulation.auto_running = True
        self.knapsack_simulation.paused = False
        self.auto_run_button.config(state=tk.DISABLED)
        self.pause_button.config(state=tk.NORMAL)
        self.run_next_step()

    def pause_auto_run(self):
        if self.knapsack_simulation.paused:
            self.knapsack_simulation.paused = False
            self.pause_button.config(text="Pause")
            self.run_next_step()
        else:
            self.knapsack_simulation.paused = True
            self.pause_button.config(text="Continue")

    def run_next_step(self):
        if self.knapsack_simulation.auto_running and not self.knapsack_simulation.paused and self.knapsack_simulation.phase != 'done':
            self.knapsack_simulation.step()
            self.root.after(self.speed_delay, self.run_next_step)

    def change_speed(self, value):
        speeds = {1: 1000, 2: 750, 3: 500, 4: 250, 5: 100}
        self.speed_delay = speeds[int(float(value))]

    def draw_tree_node(self, canvas, node, x, y, dx, dy):
        if node:
            node_text = f"{node.up:.2f}"
            
            canvas.create_oval(x-20, y-20, x+20, y+20, fill="lightblue")
            canvas.create_text(x, y, text=node_text)
            if node.parent:
                parent_x = node.parent.x
                parent_y = node.parent.y
                canvas.create_line(parent_x, parent_y, x, y)
            node.x = x
            node.y = y
            
            next_dx = max(dx // 2, 20)  # Prevent dx from becoming too small
            self.draw_tree_node(canvas, node.left, x - next_dx, y + dy, next_dx, dy)
            self.draw_tree_node(canvas, node.right, x + next_dx, y + dy, next_dx, dy)

    def update_binary_tree(self, root):
        self.tree_canvas.delete("all")
        canvas_width = self.tree_canvas.winfo_width()
        start_x = canvas_width // 2
        self.draw_tree_node(self.tree_canvas, root, start_x, 50, 100, 50)

# 创建主窗口
root = tk.Tk()
gui = KnapsackBranchBoundGUI(root)

# 运行主循环
root.mainloop()

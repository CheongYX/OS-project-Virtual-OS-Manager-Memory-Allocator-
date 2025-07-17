from flask import Flask, render_template, jsonify
import subprocess
import os
import json
from typing import Dict, Any
from collections import deque
from typing import Dict, Any, Tuple
from datetime import datetime
import time

app = Flask(__name__)

CONFIG = {
    "VM_MANAGER_PATH": os.path.join(os.path.dirname(__file__), 'bin', 'vm_manager'),
    "HISTORY_LENGTH": 15,
    "SIMULATION_DELAY": 0.5,
    "USE_SIMULATION": False
}

history = deque(maxlen=CONFIG['HISTORY_LENGTH'])

def simulate_real_time_data() -> Dict[str, Any]:
    import random
    return {
        "physical_memory": [
            {"frame": i, "page": random.randint(1, 10) if i < 3 else -1, 
             "allocated": i < 3, "last_accessed": int(time.time())}
            for i in range(4)
        ],
        "fifo_queue": [random.randint(0, 3) for _ in range(2)],
        "stats": {
            "page_faults": random.randint(5, 20),
            "hit_rate": random.uniform(0.1, 0.9)
        },
        "timestamp": datetime.now().isoformat()
    }

def get_memory_status() -> Dict[str, Any]:
    try:
        if CONFIG['USE_SIMULATION']:
            data = simulate_real_time_data()
        else:
            if not os.path.exists(CONFIG['VM_MANAGER_PATH']):
                raise FileNotFoundError(f"vm_manager not found at {CONFIG['VM_MANAGER_PATH']}")
            result = subprocess.run(
                [CONFIG['VM_MANAGER_PATH'], "--json"],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                timeout=10
            )
            if result.returncode != 0:
                raise RuntimeError(f"vm_manager failed with code {result.returncode}: {result.stderr}")
            data = json.loads(result.stdout)
        data['timestamp'] = datetime.now().isoformat()
        history.append(data)

        return {
            "current": data,
            "history": list(history),
            "meta": {
                "generated_at": datetime.now().isoformat(),
                "version": "1.0"
            }
        }
    except json.JSONDecodeError as e:
        app.logger.error(f"JSON解析失败: {str(e)}")
        return {"error": "Invalid JSON output", "details": str(e)}, 500
    except Exception as e:
        app.logger.error(f"获取内存状态失败: {str(e)}")
        return {"error": str(e)}, 500

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/status')
def api_status():
    try:
        raw_data = get_memory_status()
        if not isinstance(raw_data, dict):
            raise ValueError("Invalid data format")
        response = {
            "physical_memory": raw_data.get("physical_memory", []),
            "fifo_queue": raw_data.get("fifo_queue", []),
            "stats": raw_data.get("stats", {"page_faults": 0, "hit_rate": 0}),
            "timestamp": datetime.now().isoformat()
        }
        return jsonify(response)
        
    except Exception as e:
        return jsonify({
            "error": str(e),
            "physical_memory": [],
            "fifo_queue": [],
            "stats": {"page_faults": 0, "hit_rate": 0}
        }), 500

@app.route('/api/reset', methods=['POST'])
def reset_simulation():
    global history
    history.clear()
    return jsonify({"status": "success", "message": "History cleared"})

if __name__ == '__main__':
    app.run(
        host='0.0.0.0',
        port=5000,
        debug=True,
        use_reloader=True
    )
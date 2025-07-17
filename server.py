from flask import Flask, render_template, jsonify
import subprocess
import os
import json
from typing import Dict, Any

app = Flask(__name__)

def get_memory_status() -> Dict[str, Any]:
    try:
        cmd_path = os.path.join(os.path.dirname(__file__), 'bin', 'vm_manager')
        result = subprocess.run(
            [cmd_path, "--json"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            check=True
        )
        
        app.logger.debug(f"STDOUT: {result.stdout}")
        app.logger.debug(f"STDERR: {result.stderr}")
        
        return json.loads(result.stdout)
    except subprocess.CalledProcessError as e:
        app.logger.error(f"vm_manager执行失败: {e.stderr}")
        return {"error": "Internal Server Error"}, 500
    except json.JSONDecodeError as e:
        app.logger.error(f"JSON解析失败: {str(e)}")
        return {"error": "Invalid JSON output"}, 500
    except Exception as e:
        app.logger.error(f"未知错误: {str(e)}")
        return {"error": str(e)}, 500

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/status')
def api_status():
    status = get_memory_status()
    if "error" in status:
        return jsonify(status), 500
    return jsonify(status)

if __name__ == '__main__':
    app.run(
        host='0.0.0.0',
        port=5000,
        debug=True,
        use_reloader=True
    )
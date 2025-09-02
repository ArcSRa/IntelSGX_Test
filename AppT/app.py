import os, subprocess
from flask import Flask, abort

app = Flask(__name__)

def sgx_available():
    # 1) outil Gramine officiel (retour code 0 si OK)
    try:
        r = subprocess.run(["is-sgx-available"], capture_output=True, text=True)
        if r.returncode == 0:
            return True, r.stdout
    except FileNotFoundError:
        pass
    # 2) fallback: vérifier /dev/sgx_enclave
    return (os.path.exists("/dev/sgx_enclave"), "checked /dev/sgx_enclave")

@app.route("/")
def index():
    have_sgx, detail = sgx_available()
    mode = os.getenv("APP_MODE", "strict")  # strict | fallback
    if not have_sgx and mode == "strict":
        # Refuser de démarrer sans SGX
        abort(503, "SGX indisponible: " + detail)
    if not have_sgx and mode == "fallback":
        # Fallback sécurisé: ne pas consommer de secret ni zone chiffrée
        return "Mode non-SGX (fallback sécurisé). Aucune donnée sensible utilisée.\n"
    # SGX ok: on sert la page « secure »
    return "SGX OK: enclave active, traitement sécurisé.\n"

if __name__ == "__main__":
    # on lance le serveur
    app.run(host="0.0.0.0", port=8080)

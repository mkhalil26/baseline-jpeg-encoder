import pytest
import json

@pytest.fixture(scope="session")
def add_metric():
    # Dico category/scenario/metric
    session_data = {}
    def _add(category, scenario, metric, value, is_failed, rnd = 2):
        cat = session_data.setdefault(category, {})
        sce = cat.setdefault(scenario, {})
        sce[metric] = {"val": round(value,rnd), "fail": bool(is_failed)}
    yield _add  # On donne la fonction au test
    with open("metrics.json", "w") as f:
        json.dump(session_data, f)

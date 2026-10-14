import json
from jinja2 import Template

OUTPUT_FILE = "public/index.html"

HTML_TEMPLATE = """
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Tableau de bord de votre encodeur JPEG</title>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bulma@0.9.4/css/bulma.min.css">
    <style>
        .metric-fail { background-color: #ffeef0; color: #d31233; font-weight: bold; }
        .metric-pass { color: #257942; }
        .scenario-title { background-color: #f5f5f5; font-weight: 600; }
        .svg-container { background: white; border: 1px solid #ddd; padding: 10px; overflow: auto; max-height: 600px; }
        details.box { transition: all 0.3s ease; border: 1px solid #eee; }
        details.box[open] { border-color: #3273dc; }
        summary {
            list-style: none; /* Cache la flèche moche par défaut */
            cursor: pointer;
            outline: none;
            display: flex;
            align-items: center;
        }
        summary::before {
            content: "▶";
            display: inline-block;
            transition: transform 0.2s;
            margin-right: 15px;
            color: #3273dc;
            font-size: 0.8em;
        }
        details[open] summary::before {
            transform: rotate(90deg);
        }
        summary:hover { background-color: #fafafa; }
    </style>
</head>
<body class="has-background-light">
    <section class="section">
        <div class="container">
            <h1 class="title">Tableau de bord de votre encodeur JPEG</h1>
            <hr>
            {% for category, scenarios in session_data.items() %}
            <details class="box p-0 mb-4" open>
                <summary class="p-4 is-size-4 has-text-link has-text-weight-bold">
                    {{ category | upper }}
                </summary>
                <div class="px-5 pb-5">
                    <table class="table is-fullwidth is-hoverable is-bordered is-narrow">
                        <thead>
                            <tr class="has-background-light">
                                <th>Scénario</th>
                                {% for metric_name in (scenarios.values() | list | first).keys() %}
                                <th class="has-text-centered">{{ metric_name }}</th>
                                {% endfor %}
                            </tr>
                        </thead>
                        <tbody>
                            {% for scenario, metrics in scenarios.items() %}
                            <tr>
                                <td class="has-text-weight-bold">{{ scenario }}</td>
                                {% for metric_name, details in metrics.items() %}
                                <td class="has-text-centered {{ 'metric-fail' if details.fail else '' }}">
                                    {{ details.val }}
                                    {% if details.fail %}<br><span class="tag is-danger is-light is-size-7">FAIL</span>{% endif %}
                                </td>
                                {% endfor %}
                            </tr>
                            {% endfor %}
                        </tbody>
                    </table>
                </div>
            </details>
            {% endfor %}
            <div class="columns">
                <div class="column is-4">
                    <div class="box">
                        <h3 class="title is-5">Rapports Détaillés</h3>
                        <a href="coverage.html" class="button is-link is-fullwidth mb-2">Consulter la Couverture Code</a>
                    </div>
                </div>
                <div class="column is-8">
                    <div class="box">
                        <h3 class="title is-5">Profiling sur Biiiiiiiiig</h3>
                        <div class="svg-container">
                            {{ callgraph_svg | safe }}
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </section>
</body>
</html>
"""

def main():
    with open("merged.json", 'r') as f:
        session_data = json.load(f)
    with open("report.svg", 'r') as f:
        callgraph_svg = f.read()
    template = Template(HTML_TEMPLATE)
    rendered_html = template.render(
        session_data=session_data,
        callgraph_svg=callgraph_svg
    )
    with open(OUTPUT_FILE, "w") as f:
        f.write(rendered_html)

if __name__ == "__main__":
    main()

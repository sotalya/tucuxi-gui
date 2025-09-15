import fs from "fs";
import path from "path";
import { createCanvas, loadImage } from "canvas";

import {
  GraphFullData,
  GraphDosage,
  GraphCovariate,
  GraphMeasure,
  GraphTarget,
  GraphPredictionData,
  GraphPercentileData,
  GraphAdjustments,
  GraphAdjustment,
  GraphMouseArea,
  preparePrediction,
  preparePercs,
} from "./graphdata.js";

import { drawGraph, drawAnnotations } from "./graphing.js";

const TARGET_CODE_MAP = {
  residual: 0,
  peak: 1,
  mean: 2,
  auc: 3,
  auc24: 4,
  cumulativeAuc: 5,
  aucDividedByMic: 6,
  auc24DividedByMic: 7,
  aucOverMic: 8,
  auc24OverMic: 9,
  peakDividedByMic: 10,
  timeOverMic: 11,
  residualDividedByMic: 12,
};

const toArray = (x) => (Array.isArray(x) ? x : x ? [x] : []);
const parseDate = (s) => new Date(String(s).replace(" ", "T"));

function buildPredictionDataFromCycles(cycles) {
  const t = [],
    v = [];
  for (const c of toArray(cycles)) {
    const start = parseDate(c.start);
    const baseSec = Math.floor(start.getTime() / 1000);

    const times = Array.isArray(c.times)
      ? c.times.map(Number)
      : String(c.times || "")
          .split(",")
          .map((s) => s.trim())
          .filter(Boolean)
          .map(Number);

    const values = Array.isArray(c.values)
      ? c.values.map(Number)
      : String(c.values || "")
          .split(",")
          .map((s) => s.trim())
          .filter(Boolean)
          .map(Number);

    const len = Math.min(times.length, values.length);
    for (let i = 0; i < len; i++) {
      t.push(baseSec + times[i] * 3600);
      v.push(values[i]);
    }
  }
  const data = new GraphPredictionData(t, v);
  if (t.length > 1) {
    const idx = t.map((_, i) => i).sort((i, j) => t[i] - t[j]);
    data.time = idx.map((i) => t[i]);
    data.value = idx.map((i) => v[i]);
  }
  data.isValid = t.length > 0;
  return data;
}

function buildAdjustments(adjustmentsContainer) {
  const rev = new GraphAdjustments();
  const list = adjustmentsContainer?.adjustment
    ? toArray(adjustmentsContainer.adjustment)
    : toArray(adjustmentsContainer);

  let bestIdx = -1,
    bestScore = -Infinity,
    anySelected = false;

  list.forEach((adj, idx) => {
    const cycles = toArray(adj?.cycleDatas?.cycleData);
    const pd = buildPredictionDataFromCycles(cycles);
    if (!pd.isValid) return;

    const g = new GraphAdjustment();
    g.predictionData = pd;
    if (adj.selected) {
      g.predictionData.selected = true;
      anySelected = true;
    }
    const s = Number(adj?.score);
    if (Number.isFinite(s) && s > bestScore) {
      bestScore = s;
      bestIdx = rev.size; // index après append
    }
    rev.append(g);
  });

  if (!anySelected && rev.size > 0) {
    const idx = bestIdx >= 0 ? bestIdx : 0;
    rev.get(idx).predictionData.selected = true;
  }

  rev.isValid = rev.size > 0;
  return rev;
}

function buildGraphFullData(json) {
  const obj = new GraphFullData();

  obj.dosages.push(new GraphDosage());
  obj.currentDosage = obj.dosages[0];

  // --- Covariates (A voir si on enlève) ---
  const cycles =
    json?.tucuxiComputation?.responses?.response?.[0]?.dataAdjustment
      ?.cycleDatas?.cycleData ||
    json?.cycleDatas ||
    [];
  for (const c of cycles) {
    if (c.start && c.covariates) {
      obj.pvars.push(new GraphCovariate(parseDate(c.start)));
    }
  }

  // --- Measures (A voir si on enlève) ---
  // Mais acutellement il n'y a pas de "measures" ou de samples dans le JSON

  // --- Targets ---
  // Actuellement on a que les targetEvaluation sans aucune information sur les best min et max des targets
  //   On a juste la targetEvaluation.value, donc pour le moent je fais juste un * pour le min et max pour estimer
  const evalRaw =
    json?.tucuxiComputation?.responses?.response?.[0]?.dataAdjustment
      ?.adjustments?.adjustment?.targetEvaluations?.targetEvaluation ||
    json?.targets ||
    null;

  const evalArray = Array.isArray(evalRaw) ? evalRaw : evalRaw ? [evalRaw] : [];

  for (const te of evalArray) {
    const key = te.targetType;
    const code = TARGET_CODE_MAP[key];
    if (code == null) continue;
    const tgt = te.value;
    if (tgt == null) continue;

    const min = tgt * 0.8; // Hasardous, but we don't have min/max in the JSON
    const max = tgt * 1.2; // Hasardous, but we don't have min/max in the JSON

    obj.targets.push(new GraphTarget(code, min, tgt, max));
  }

  // --- Prediction (Population) ---
  const mainPrediction = buildPredictionDataFromCycles(cycles);
  obj.popP.predictive.predictionData = mainPrediction;
  preparePrediction(obj.popP);

  // --- Percentiles (if any) ---
  // Actuellement aucun dans le json
  // const percList = buildPercentiles(json?.percentiles || []);
  // for (const p of percList) obj.popercsP.append(p);
  // if (obj.popercsP.size) {
  //     obj.popercsP.isValid = true;
  //     preparePercs(obj.popercsP);
  // }

  // --- Adjustments ---
  const rev = buildAdjustments(
    json?.tucuxiComputation?.responses?.response?.[0]?.dataAdjustment
      ?.adjustments
  );
  if (rev?.isValid) obj.revP = rev;

  console.log(obj);
  return obj;
}

function computeTimeBounds(obj) {
  const ts = [];
  const pushPD = (pd) => {
    if (pd?.isValid && Array.isArray(pd.time) && pd.time.length)
      ts.push(...pd.time);
  };
  pushPD(obj?.popP?.predictive?.predictionData);
  if (obj?.revP?.size)
    for (let i = 0; i < obj.revP.size; i++)
      pushPD(obj.revP.get(i)?.predictionData);

  if (!ts.length) return;
  obj.timestart = new Date(Math.min(...ts) * 1000);
  obj.timeend = new Date(Math.max(...ts) * 1000);
}

export async function renderFromJson(
  json,
  {
    scale = 2,
    width = 1200,
    height = 800,
    outPath = "output.png",
    iconCovariatesPath = "covariates_disabled_mini.png",
    iconDosagesPath = "dosages_disabled_mini.png",
  } = {}
) {
  const obj = buildGraphFullData(json);
  obj.scale = scale;

  obj.mArea = new GraphMouseArea();
  obj.mArea.mouseX = 300 * obj.scale;
  obj.mArea.mouseY = 200 * obj.scale;
  obj.mArea.containsMouse = true;
  obj.mArea.isMouseOver = true;
  obj.mArea.tooltipX = 300 * obj.scale;
  obj.mArea.tooltipY = 200 * obj.scale;

  computeTimeBounds(obj);

  const canvas = createCanvas(width, height);
  obj.updateChartDimensions(canvas);
  obj.canvas = canvas;
  obj.annotationsCanvas = canvas;
  obj.clockCanvas = canvas;

  try {
    obj.img_covariates_disabled_mini = await loadImage(iconCovariatesPath);
  } catch (_) {}
  try {
    obj.img_dosages_disabled_mini = await loadImage(iconDosagesPath);
  } catch (_) {}

  drawGraph(obj);
  drawAnnotations(obj);

  const buffer = canvas.toBuffer("image/png");
  fs.writeFileSync(outPath, buffer);
  return outPath;
}

if (
  import.meta &&
  import.meta.url &&
  process.argv[1] === new URL(import.meta.url).pathname
) {
  (async () => {
    const input = process.argv[2];
    if (!input) {
      console.error("Usage: node render_from_json.js path/to/input.json");
      process.exit(1);
    }
    const raw = fs.readFileSync(path.resolve(input), "utf8");
    const json = JSON.parse(raw);

    const out = await renderFromJson(json, {
      scale: 2,
      width: 1200,
      height: 800,
      outPath: "output_from_json.png",
    });

    console.log("Rendu sauvegardé :", out);
  })();
}

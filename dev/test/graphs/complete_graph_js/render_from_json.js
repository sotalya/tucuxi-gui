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
  GraphPredictionResult,
  GraphPredictive,
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

const toNum = (v, fallback = NaN) => {
  const n = typeof v === 'string' ? Number(v) : (typeof v === 'number' ? v : NaN);
  return Number.isFinite(n) ? n : fallback;
};

const parseNums = (raw) => {
  if (raw == null) return [];
  if (typeof raw === 'string') return raw.split(',').map(s => s.trim()).filter(Boolean).map(Number);
  if (Array.isArray(raw)) return raw.map(Number);
  return [];
};

function addPredictionData(a, d, c, e) {
  for (var b = 0; b < c.length; b++)
    a.predictionData.time.push(3600 * c[b] + d.getTime() / 1e3),
      a.predictionData.value.push(e[b]);
  return a.predictionData.troughs.push(a.predictionData.value.length - 1), a;
}

function processAdjData(graphFullData, adjData) {
  adjData.forEach((adjGroup, index) => {
    let e = new GraphAdjustment();
    
    adjGroup.forEach(adj => {
      if (!Array.isArray(adj) || adj.length < 3) {
        console.warn("Invalid adjustment data", adj);
        return;
      }
      e = addPredictionData(e, parseDate(adj[0]), adj[1], adj[2]);
    });
    
    e.predictionData.isValid = true;
    graphFullData.revP.append(e);
    
    // if (index === 0) {
    //   let f = document.getElementById("canBestAdj");
    //   if (f) {
    //     graphFullData.updateChartDimensions(f);
    //     graphFullData.canvas = f;
    //     graphFullData.annotationsCanvas = f;
    //     graphFullData.clockCanvas = f;
    //   }
    // }
  });
}

function processAprioriPercentiles(graphFullData, aprioriPercentiles) {
  const percentileMap = {};

  aprioriPercentiles.forEach(percentile => {

    if (!aprioriPercentiles || !Array.isArray(aprioriPercentiles) || aprioriPercentiles.length === 0) {
      console.warn("Invalid apriori percentiles", aprioriPercentiles);
      return;
    }

    const rank = Number(percentile[0]);
    if (![5, 10, 25, 50, 75, 90, 95].includes(rank)) {
      console.warn("Invalid percentile rank", percentile[0]);
      return;
    }

    const startTime = new Date(percentile[1].replace(" ", "T"));
    if (isNaN(startTime.getTime())) {
      console.warn("Invalid percentile start date", percentile[1]);
      return;
    }

    const rawTimes = typeof percentile[2] === "string" 
      ? percentile[2].split(',').map(Number) 
      : percentile[2];

    const values = typeof percentile[3] === "string" 
      ? percentile[3].split(',').map(Number) 
      : percentile[3];

    const computedTimes = rawTimes.map(t => 3600 * t + startTime.getTime() / 1e3);

    if (!percentileMap[rank]) {
      percentileMap[rank] = {
        predictionData: new GraphPredictionData([], []),
        rank: rank
      };
    }

    percentileMap[rank].predictionData.time.push(...computedTimes);
    percentileMap[rank].predictionData.value.push(...values);
  });

  for (const rank in percentileMap) {
    const entry = percentileMap[rank];

    entry.predictionData.isValid = true;

    const p = new GraphPercentileData();
    p.percentile = entry.rank;
    p.predictionData = entry.predictionData;
    p.isValid = true;

    graphFullData.aprpercsP.append(p);
  }

  graphFullData.aprpercsP.isValid = true;
  preparePercs(graphFullData.aprpercsP);
}

function processPredictionData(graphFullData, predictionData, key) {
  if (!predictionData || !Array.isArray(predictionData) || predictionData.length === 0) {
    console.warn("Invalid prediction data", predictionData);
    return;
  }
  let graphPredictionResult = new GraphPredictionResult();
  let graphPredictive = new GraphPredictive();

  predictionData.forEach(predictions => {
    let [dateStr, xValues, yValues] = predictions;
    let date = parseDate(dateStr);

    if (isNaN(date.getTime())) {
      console.warn("Invalid date in prediction set", dateStr, key);
      return;
    }

    xValues.forEach((x, idx) => {
      graphPredictive.predictionData.time.push(3600 * x + date.getTime() / 1e3);
      graphPredictive.predictionData.value.push(yValues[idx]);
    });
  });

  graphPredictive.predictionData.isValid = true;
  graphPredictionResult.predictive = graphPredictive;
  graphFullData[key] = graphPredictionResult;

  preparePrediction(graphFullData[key]);
}

function buildAdjustments(data) {
  const responses = toArray(data?.tucuxiComputation?.responses?.response);
  const adjustments = responses
    .filter(r => r?.requestType === 'adjustment')
    .flatMap(r => toArray(r?.dataAdjustment?.adjustments?.adjustment));


  if (adjustments.length === 0) {
    return { adjData: [], adjStartDate: null, adjEndDate: null };
  }

  const getAdjScore = (adj) => {
    const primary = toNum(adj?.score, NaN);
    if (Number.isFinite(primary)) return primary;
  };

  const bestAdjustment = adjustments.reduce((best, curr) =>
    getAdjScore(curr) > getAdjScore(best) ? curr : best
  );

  let adjdates = [];

  const range = bestAdjustment?.dosageHistory?.dosageTimeRange;
  const s = range?.start;
  const e = range?.end;
  if (s) adjdates.push(parseDate(s));
  if (e) adjdates.push(parseDate(e));

  const adjData = toArray(bestAdjustment?.cycleDatas?.cycleData).map(cycle => {
    const rawTimes =
      cycle?.times;

    const rawValues =
      cycle?.values;

    const times_list = parseNums(rawTimes);
    const values_list = parseNums(rawValues);

    return [
      cycle?.start ?? null,
      times_list,
      values_list
    ];
  });

  return { adjData, adjdates };
}


function buildTargets(json) {
  const responses = toArray(json?.tucuxiComputation?.responses?.response)
    .filter(r => r?.requestType === 'adjustment');

  const targets = responses.flatMap(r => {
    const fromTargets =
      toArray(r?.dataAdjustment?.targets?.target ?? r?.dataAdjustment?.targets);

    const all = [...fromTargets];

    return all.map(t => {
      const type = t?.type ?? t?.targetType ?? null;

      const min  = toNum(t?.min  ?? t?.lower    ?? t?.minValue);
      const best = toNum(t?.best ?? t?.value    ?? t?.target ?? t?.aim);
      const max  = toNum(t?.max  ?? t?.upper    ?? t?.maxValue);

      return [type, min, best, max];
    });
  });

  return targets;
}

function buildSamples(json) {
  return [];
}

function buildPecentiles(json) {
  const responses = toArray(json?.tucuxiComputation?.responses?.response)
    .filter(r => r?.requestType === 'percentiles');

  const percentilesDates = [];
  const percentilesData = [];

  for (const resp of responses) {
    const plist = toArray(resp?.dataPercentiles?.percentile);

    for (const p of plist) {
      const rank = toNum?.(p?.rank, NaN) ?? (() => {
        const n = parseFloat(p?.rank);
        return Number.isFinite(n) ? n : NaN;
      })();

      const cycles = toArray(p?.cycleDatas?.cycleData);

      for (const cycle of cycles) {
        const start = cycle?.start ?? null
        const end = cycle?.end ?? null

        if (start) percentilesDates.push(parseDate(start));
        if (end)   percentilesDates.push(parseDate(end));

        const rawTimes =
          cycle?.times;

        const rawValues =
          cycle?.values;

        const times_list = parseNums(rawTimes);
        const values_list = parseNums(rawValues);

        percentilesData.push([
          rank,
          start,
          times_list,
          values_list
        ]);
      }
    }

  }

  return { percentilesData, percentilesDates };
}


function buildPredictions(json) {
  const responses = toArray(json?.tucuxiComputation?.responses?.response)
    .filter(r => r?.requestType === 'prediction');

  const cycles = responses.flatMap(r =>
    toArray(r?.dataPrediction?.cycleDatas?.cycleData)
  );

  if (cycles.length === 0) {
    return { predictionData: [], dates: [] };
  }

  let predDates = [];
  const predictionData = cycles.map(cycle => {
    if (cycle?.start) predDates.push(parseDate(cycle.start));
    if (cycle?.end)   predDates.push(parseDate(cycle.end));

    const rawTimes =
      cycle?.times ??
      cycle?.timePoints?.time ??
      cycle?.points?.times ?? null;

    const rawValues =
      cycle?.values ??
      cycle?.concentrations?.value ??
      cycle?.points?.values ?? null;

    const times_list = parseNums(rawTimes);
    const values_list = parseNums(rawValues);

    return [
      cycle?.start ?? null,
      times_list,
      values_list
    ];
  });

  return { predictionData, predDates } ;
}


function manageDates(adjDates, predDates, percentilesDates) {
  const all = [...(adjDates || []), ...(predDates || []), ...(percentilesDates || [])];

  const toTime = (d) => {
    if (!d) return NaN;
    if (d instanceof Date) return d.getTime();
    if (typeof d === 'number') return Number.isFinite(d) ? d : NaN;
    if (typeof d === 'string') {
      const dt = new Date(d);             // "2023-01-20T12:00:00" = heure locale ; "2023-01-20T12:00:00Z" = UTC
      const t = dt.getTime();
      return Number.isNaN(t) ? NaN : t;
    }
    return NaN;
  };

  let minT = Infinity, maxT = -Infinity;

  for (const d of all) {
    const t = toTime(d);
    if (!Number.isFinite(t)) continue;
    if (t < minT) minT = t;
    if (t > maxT) maxT = t;
  }

  return {
    earliest: Number.isFinite(minT) ? new Date(minT) : null,
    latest:   Number.isFinite(maxT) ? new Date(maxT) : null,
  };
}

function buildGraphFullData2(json) {
  const obj = new GraphFullData();

  // --- Prepare data ---
  const { adjData, adjdates } = buildAdjustments(json);

  const { predictionData, predDates } = buildPredictions(json);

  const { percentilesData, percentilesDates } = buildPecentiles(json);

  const { earliest, latest } = manageDates(adjdates, predDates);

  const targets = buildTargets(json);

  // -- GraphFullData Data --
  obj.timestart = earliest;
  obj.timeend = latest;
  obj.revP.isValid = true;
  obj.apoP.isValid = true;
  obj.aprP.isValid = true;
  obj.scale = 1;

  // --- Samples if any ---

  // --- AprP percentiles if any ---
  processAprioriPercentiles(obj, percentilesData);

  // --- ApoP percentiles if any ---

  // --- AprP prediction if any ---
  processPredictionData(obj, predictionData, 'aprP');

  // --- ApoP prediction if any ---

  // --- Adjustments if any ---
  processAdjData(obj, adjData);

  // --- Targets if any ---


  console.log("GraphFullData", obj);
  return obj;
}

// function computeTimeBounds(obj) {
//   const ts = [];
//   const pushPD = (pd) => {
//     if (pd?.isValid && Array.isArray(pd.time) && pd.time.length)
//       ts.push(...pd.time);
//   };
//   pushPD(obj?.popP?.predictive?.predictionData);
//   if (obj?.revP?.size)
//     for (let i = 0; i < obj.revP.size; i++)
//       pushPD(obj.revP.get(i)?.predictionData);

//   if (!ts.length) return;
//   obj.timestart = new Date(Math.min(...ts) * 1000);
//   obj.timeend = new Date(Math.max(...ts) * 1000);
// }

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
  const obj = buildGraphFullData2(json);
  obj.scale = scale;

  obj.mArea = new GraphMouseArea();
  obj.mArea.mouseX = 300 * obj.scale;
  obj.mArea.mouseY = 200 * obj.scale;
  obj.mArea.containsMouse = true;
  obj.mArea.isMouseOver = true;
  obj.mArea.tooltipX = 300 * obj.scale;
  obj.mArea.tooltipY = 200 * obj.scale;

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

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

// function processAdjData(graphFullData, adjData) {
//   if (!Array.isArray(adjData) || adjData.length === 0) return;

//   const isTuple = (x) =>
//     Array.isArray(x) &&
//     (typeof x[0] === "string" || x[0] instanceof Date) &&
//     Array.isArray(x[1]) && Array.isArray(x[2]);

//   const isGroup = (x) => Array.isArray(x) && Array.isArray(x[0]) && isTuple(x[0]);

//   const pushCycle = (adj, start, times, values) => {
//     if (!start || !Array.isArray(times) || !Array.isArray(values)) return;
//     if (times.length !== values.length) {
//       console.warn("Adjustment times/values length mismatch:", { start, times: times.length, values: values.length });
//       return;
//     }
//     addPredictionData(adj, parseDate(start), times, values);
//   };

//   for (const entry of adjData) {
//     const adj = new GraphAdjustment();

//     // entry peut être un triplet ou un groupe de triplets
//     if (isGroup(entry)) {
//       for (const [start, times, values] of entry) {
//         pushCycle(adj, start, times, values);
//       }
//     } else if (isTuple(entry)) {
//       const [start, times, values] = entry;
//       pushCycle(adj, start, times, values);
//     } else {
//       console.warn("Adjustment entry mal formé:", entry);
//       continue;
//     }

//     if (adj.predictionData.time.length > 0) {
//       adj.predictionData.isValid = true;
//       graphFullData.revP.append(adj);
//     }
//   }

//   graphFullData.revP.isValid = graphFullData.revP.size() > 0;
// }

function processAdjData(graphFullData, adjData) {
  if (!Array.isArray(adjData) || adjData.length === 0) return;

  adjData.forEach((entry) => {
  
    const e = new GraphAdjustment();

    if (Array.isArray(entry[0])) {
      entry.forEach(([start, times, values]) => {
        if (!start || !Array.isArray(times) || !Array.isArray(values)) return;
        addPredictionData(e, parseDate(start), times, values);
      });
    } else {
      const [start, times, values] = entry;
      if (start && Array.isArray(times) && Array.isArray(values)) {
        addPredictionData(e, parseDate(start), times, values);
      }
    }

    e.predictionData.isValid = true;
    graphFullData.revP.append(e);
  });

  graphFullData.revP.isValid = true;
}

function processTargets(obj, targets) {
  if (!obj || !Array.isArray(targets)) return;

  for (const t of targets) {
    if (!Array.isArray(t) || t.length < 4) {
      console.warn("Target invalid (Wanted: [type,min,best,max]) :", t);
      continue;
    }

    let [type, min, best, max] = t;

    let code = undefined;
    if (typeof type === "string") {
      code = TARGET_CODE_MAP[type.trim()];
    } else if (Number.isFinite(type)) {
      code = type | 0;
    }

    if (!Number.isFinite(code)) {
      console.warn("Target type unknown :", type, "(skipped)");
      continue;
    }

    const nmin  = Number(min);
    const nbest = Number(best);
    const nmax  = Number(max);
    if (![nmin, nbest, nmax].every(Number.isFinite)) {
      console.warn("Non numerical limits", t);
      continue;
    }

    let lo = nmin, mid = nbest, hi = nmax;
    if (lo > hi) { const tmp = lo; lo = hi; hi = tmp; }
    if (mid < lo) mid = lo;
    if (mid > hi) mid = hi;

    obj.targets.push(new GraphTarget(code, lo, mid, hi));
  }
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
    .filter(r => r?.requestType === 'target');

  const targets = responses.flatMap(r => {
    const fromTargets =
      toArray(r?.dataTarget?.targets?.target ?? r?.dataTarget?.targets);

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
  const { earliest, latest } = manageDates(adjdates, predDates, percentilesDates);
  const targets = buildTargets(json);

  // -- GraphFullData Data --
  obj.timestart = earliest;
  obj.timeend = latest;

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
  processTargets(obj, targets);
  console.log("Targets processed", obj.targets);
  console.log("Has Target", obj.hasTargets)

  // console.log("GraphFullData", obj);
  return obj;
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
  const obj = buildGraphFullData2(json);
  obj.scale = scale;

  obj.mArea = new GraphMouseArea();
  obj.mArea.mouseX = 300 * obj.scale;
  obj.mArea.mouseY = 200 * obj.scale;
  obj.mArea.containsMouse = false;
  obj.mArea.isMouseOver = false;
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

  const start = obj.timestart?.getTime() / 1000;
  const end   = obj.timeend?.getTime() / 1000;

  obj.getViewRangeMin = () => start ?? (Date.now()/1000 - 24*3600);
  obj.getViewRangeMax = () => end   ?? (Date.now()/1000);

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

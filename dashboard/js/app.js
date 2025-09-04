// Firebase конфігурація
const firebaseConfig = {
  //Insert your Data
};

firebase.initializeApp(firebaseConfig);
const db = firebase.database();

function updateClock() {
  const now = new Date();
  const timeString = now.toLocaleTimeString('uk-UA');
  document.getElementById('clock').innerText = timeString;

  // Графік часу
  db.ref("Feeder/ScheduledTime").once("value").then(snapshot => {
    const scheduled = snapshot.val();
    document.getElementById("saved-time").innerHTML = `<i class="far fa-clock"></i> Обраний час: ${scheduled || '--:--'}`;
  });

  // Останні годування
  db.ref("Feeder/LastFedManual").once("value").then(snap => {
    const val = snap.val();
    if (val) {
      const date = new Date(Number(val) * 1000);
      document.getElementById("last-fed-manual").innerHTML = `<i class="fas fa-history"></i> Останнє ручне годування: ${date.toLocaleString('uk-UA')}`;
    }
  });

  db.ref("Feeder/LastFedAuto").once("value").then(snap => {
    const val = snap.val();
    if (val) {
      const date = new Date(Number(val) * 1000);
      document.getElementById("last-fed-auto").innerHTML = `<i class="fas fa-history"></i> Останнє автогодівля: ${date.toLocaleString('uk-UA')}`;
    }
  });

  db.ref("Feeder/Alert").once("value").then(snap => {
    const alertMsg = snap.val() || "OK";
    const alertEl = document.getElementById("alert-message");
    alertEl.innerText = `Статус: ${alertMsg}`;
    
    if (alertMsg === "OK") {
      alertEl.style.background = "rgba(40, 167, 69, 0.15)";
      alertEl.style.color = "var(--success)";
      alertEl.style.border = "1px solid rgba(40, 167, 69, 0.3)";
    } else {
      alertEl.style.background = "rgba(255, 87, 51, 0.15)";
      alertEl.style.color = "var(--danger)";
      alertEl.style.border = "1px solid rgba(255, 87, 51, 0.3)";
    }
  });

  db.ref("Feeder/Status").once("value").then(snap => {
    const status = snap.val();
    const el = document.getElementById("servo-status");
    if (status === "Feed") {
      el.innerText = "Стан: Годівниця активна";
      el.className = "status-badge status-active";
    } else {
      el.innerText = "Стан: Очікування";
      el.className = "status-badge status-inactive";
    }
  });
}

setInterval(updateClock, 1000);
window.onload = updateClock;

function feedNow() {
  db.ref("Feeder").update({ Status: "Feed" });
}

function setFeedingTime() {
  const selectedTime = document.getElementById("feedTime").value;
  if (selectedTime) {
    db.ref("Feeder").update({ ScheduledTime: selectedTime });
    alert("Час збережено: " + selectedTime);
  } else {
    alert("Оберіть час!");
  }
}

// Температура
let tempChart;
db.ref("SensorHistory").on("value", snapshot => {
  const data = snapshot.val();
  if (!data) return;

  const labels = [], temps = [];
  Object.entries(data).sort((a, b) => a[0] - b[0]).forEach(([t, v]) => {
    labels.push(new Date(Number(t) * 1000).toLocaleTimeString('uk-UA'));
    temps.push(v);
  });

  const ctx = document.getElementById('tempChart').getContext('2d');
  if (!tempChart) {
    tempChart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: labels,
        datasets: [{
          label: "Температура (°C)",
          data: temps,
          borderColor: "#FF5733",
          backgroundColor: "rgba(255, 87, 51, 0.1)",
          borderWidth: 2,
          tension: 0.3,
          fill: true,
          pointBackgroundColor: "#FF5733",
          pointBorderColor: "#fff",
          pointHoverRadius: 6
        }]
      },
      options: { 
        responsive: true,
        scales: {
          y: {
            beginAtZero: false,
            grid: {
              color: "rgba(0, 0, 0, 0.05)"
            }
          },
          x: {
            grid: {
              display: false
            }
          }
        },
        plugins: {
          legend: {
            labels: {
              font: {
                family: "'Roboto', sans-serif",
                size: 14
              }
            }
          }
        }
      }
    });
  } else {
    tempChart.data.labels = labels;
    tempChart.data.datasets[0].data = temps;
    tempChart.update();
  }
});

// Вода
let waterChart;
db.ref("SensorWaterHistory").on("value", snapshot => {
  const data = snapshot.val();
  if (!data) return;

  const labels = [], values = [];
  Object.entries(data).sort((a, b) => a[0] - b[0]).forEach(([t, v]) => {
    labels.push(new Date(Number(t) * 1000).toLocaleTimeString('uk-UA'));
    values.push(v);
  });

  const ctx = document.getElementById('waterChart').getContext('2d');
  if (!waterChart) {
    waterChart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: labels,
        datasets: [{
          label: "Рівень води (%)",
          data: values,
          borderColor: "#007BFF",
          backgroundColor: "rgba(0, 123, 255, 0.1)",
          borderWidth: 2,
          tension: 0.3,
          fill: true,
          pointBackgroundColor: "#007BFF",
          pointBorderColor: "#fff",
          pointHoverRadius: 6
        }]
      },
      options: { 
        responsive: true,
        scales: {
          y: {
            beginAtZero: true,
            max: 100,
            grid: {
              color: "rgba(0, 0, 0, 0.05)"
            }
          },
          x: {
            grid: {
              display: false
            }
          }
        },
        plugins: {
          legend: {
            labels: {
              font: {
                family: "'Roboto', sans-serif",
                size: 14
              }
            }
          }
        }
      }
    });
  } else {
    waterChart.data.labels = labels;
    waterChart.data.datasets[0].data = values;
    waterChart.update();
  }
});

// Історія годувань
db.ref("Feeder/FeedLog").on("value", snapshot => {
  const data = snapshot.val();
  const list = document.getElementById("feed-log");
  list.innerHTML = "";
  if (!data) {
    list.innerHTML = '<li class="history-item"><span class="history-time">Немає записів</span></li>';
    return;
  }

  Object.entries(data).sort((a, b) => b[0] - a[0]).slice(0, 10).forEach(([t, v]) => {
    const time = new Date(Number(t) * 1000).toLocaleString('uk-UA');
    const item = document.createElement("li");
    item.className = "history-item";
    item.innerHTML = `
      <span class="history-time">${time}</span>
      <span class="history-type ${v === "Manual" ? 'manual' : 'auto'}">${v === "Manual" ? "Ручне" : "Авто"}</span>
    `;
    list.appendChild(item);
  });
});
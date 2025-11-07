const pw = document.getElementById('pw');
const btn = document.getElementById('check');
const result = document.getElementById('result');

const toggle = document.getElementById('togglePw');
toggle.addEventListener('click', () => {
  const isHidden = pw.type === 'password';
  pw.type = isHidden ? 'text' : 'password';
  toggle.textContent = isHidden ? 'Hide' : 'Show';
  toggle.setAttribute('aria-pressed', String(isHidden));
  toggle.setAttribute('aria-label', isHidden ? 'Hide password' : 'Show password');
});

btn.addEventListener('click', async () => {
  const r = await fetch('/api/estimate', {
    method: 'POST',
    headers: {'Content-Type':'application/json'},
    body: JSON.stringify({ password: pw.value })
  });
  const data = await r.json();
  result.innerHTML = `
    <p><strong>Score:</strong> ${data.score} / 4</p>
    <p><strong>Verdict:</strong> ${data.verdict}</p>
    <p><strong>Entropy (bits):</strong> ${data.entropy_bits.toFixed(1)}</p>
    <ul>${(data.feedback||[]).map(s=>`<li>${s}</li>`).join('')}</ul>
  `;
});

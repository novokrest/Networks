<%--
  Created by IntelliJ IDEA.
  User: novokrest
  Date: 12/10/14
  Time: 11:45 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head lang="en">
  <meta charset="UTF-8">
  <title>${it.bankName}</title>
</head>
<body>
<div align="center">
  <img src="/resources/${it.bankImg}" width="300">
  <br/><br/>
  <table>
      <td><img src="/resources/${it.currencyImg}" height="100"></td>
      <td width="100" align="center">BUY<br/>${it.buyValue} ruble(s)</td>
      <td width="100" align="center">SELL<br/>${it.sellValue} ruble(s)</td>
    </tr>
  </table>
</div>
</body>
</html>

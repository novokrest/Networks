package spbau.novokreshchenov.network.hw1;

import com.sun.jersey.api.MessageException;
import com.sun.jersey.api.view.Viewable;
import spbau.novokreshchenov.network.hw1.Exceptions.TaskNotFoundException;
import spbau.novokreshchenov.network.hw1.Exceptions.XmlMessageException;
import spbau.novokreshchenov.network.hw1.Tasks.DeleteTask;
import spbau.novokreshchenov.network.hw1.Tasks.PostTask;
import spbau.novokreshchenov.network.hw1.Tasks.PutResult;
import spbau.novokreshchenov.network.hw1.Tasks.PutTask;

import javax.ws.rs.*;
import javax.ws.rs.core.Response;
import java.io.File;
import java.net.URISyntaxException;
import java.util.HashMap;
import java.util.Map;


@Path("/exchange")
public class ExchangeRateResource {
    private static final ExchangeRateSearcher searcher = new ExchangeRateSearcher();
    private static final ExchangeRateTaskManager taskManager = new ExchangeRateTaskManager(searcher);

    @GET
    @Produces({"text/html", "application/html"})
    public Response getWelcome() {
        return Response.ok().entity("<h1>Welcome to exchange rate site!<h1>").build();
    }

    @GET
    @Path("/{id}")
    @Produces({"text/html", "application/html"})
    public Response getTaskResultInHtml(@PathParam("id") String taskId) {
        ExchangeRateTaskResult result;
        try {
            result = taskManager.getTaskResult(taskId);
        }
        catch (MessageException exception) {
            return Response.status(Response.Status.NOT_FOUND)
                    .entity("<html>" + "<title>404</title>" + "<body><h1>" + exception.getMessage() + "</h1></body>" + "</html>")
                    .build();
        }

        Map<String, Object> jspParams = new HashMap<String, Object>();
        jspParams.put("bankName", result.getBankName());
        jspParams.put("bankImg", getBankImgByName(result.getBankName()));
        jspParams.put("currencyImg", getCurrencyImgByName(result.getCurrency()));
        jspParams.put("buyValue", result.getBuyValue());
        jspParams.put("sellValue", result.getSellValue());

        return Response.ok(new Viewable("/bank-exchange-rate", jspParams)).build();
    }

    @GET
    @Path("/{id}")
    @Produces("application/xml")
    public Response getTaskResultInXml(@PathParam("id") String taskId) {
        ExchangeRateTaskResult result;
        try {
            result = taskManager.getTaskResult(taskId);
        }
        catch (MessageException exception) {
            return Response.status(Response.Status.NOT_FOUND)
                    .entity(new XmlMessageException(exception.getMessage()))
                    .type("application/xml")
                    .build();
        }
        return Response.ok().entity(result).type("application/json").build();
    }

    @GET
    @Path("/{id}")
    @Produces({"application/json"})
    public Response getTaskResultInJson(@PathParam("id") String taskId) {
        ExchangeRateTaskResult result;
        try {
            result = taskManager.getTaskResult(taskId);
        }
        catch (MessageException exception) {
            return Response.status(Response.Status.NOT_FOUND)
                    .entity(new XmlMessageException(exception.getMessage()))
                    .type("application/json")
                    .build();
        }
        return Response.ok().entity(result).type("application/json").build();
    }

    @GET
    @Path("/{id}")
    @Produces("text/plain")
    public Response getTaskResultInPlain(@PathParam("id") String taskId) {
        ExchangeRateTaskResult result;
        try {
            result = taskManager.getTaskResult(taskId);
        }
        catch (MessageException exception) {
            return Response.status(Response.Status.NOT_FOUND)
                    .entity(exception.getMessage())
                    .type("text/plain")
                    .build();
        }
        return Response.ok().entity(result.toString()).type("text/plain").build();
    }


    @PUT
    @Consumes({"text/json", "application/json"})
    @Produces({"text/json", "application/json"})
    public Response putTaskInJson(PutTask task) {
        String taskId = taskManager.putTask(new ExchangeRateTask(task.bank, task.currency));

        return Response.ok(new PutResult(taskId), "application/json").build();
    }

    @POST
    @Path("/{id}")
    @Consumes({"text/json", "application/json"})
    public Response postTaskInJson(
            @PathParam("id") String id,
            PostTask task) {
        try {
            taskManager.postTask(id, new ExchangeRateTask(task.bank, task.currency));
        }
        catch (MessageException exception) {
            return Response.status(404)
                    .entity(new XmlMessageException(exception.getMessage()))
                    .type("application/json")
                    .build();
        }

        return Response.ok().build();
    }

    @DELETE
    @Consumes({"text/json", "application/json"})
    public Response deleteTaskInJson(DeleteTask task) {
        try {
            taskManager.deleteTask(task.id);
        }
        catch (TaskNotFoundException exception) {
            return Response.status(404)
                    .entity(new XmlMessageException(exception.getMessage()))
                    .type("application/json")
                    .build();
        }

        return Response.ok().build();
    }

    private String getBankImgByName(String bankName) {
        return searcher.getBankByName(bankName).getIconPath();
    }

    private String getCurrencyImgByName(String currency) {
        return searcher.getCurrencyByName(currency).getIconPath();
    }

    private File getResourceImage(String imageName) {
        File f = null;
        try {
            f = new File(this.getClass().getResource("/images/" + imageName).toURI());
        } catch (URISyntaxException e) {
        }

        return f.exists() ? f : null;
    }
}
